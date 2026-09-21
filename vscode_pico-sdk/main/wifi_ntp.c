#include "wifi_ntp.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define NTP_SERVER "pool.ntp.org"
#define NTP_MSG_LEN 48
#define NTP_PORT 123
#define NTP_TIMEOUT_MS 5000

wifi_config_t wifi_config = {
    .ssid = "",
    .password = "",
    .connected = false,
    .last_sync = 0,
    .sync_interval_ms = 30 * 60 * 1000  // 30 minutos padrão
};

static struct udp_pcb* ntp_pcb = NULL;
static uint32_t ntp_response_received = 0;

// =====================================================================
// Callback para resposta do servidor NTP
void ntp_recv_callback(void* arg, struct udp_pcb* pcb, struct pbuf* p,
                       const ip_addr_t* addr, uint16_t port) {
    if (p->len == NTP_MSG_LEN) {
        uint8_t mode = pbuf_get_at(p, 0) & 0x7;
        if (mode == 4 || mode == 5) {  // Server ou Broadcast
            uint8_t* ntp_msg = (uint8_t*)p->payload;
            
            // Extrai timestamp (segundos desde 1900-01-01)
            uint32_t ntp_timestamp = 0;
            ntp_timestamp |= ((uint32_t)ntp_msg[40]) << 24;
            ntp_timestamp |= ((uint32_t)ntp_msg[41]) << 16;
            ntp_timestamp |= ((uint32_t)ntp_msg[42]) << 8;
            ntp_timestamp |= ((uint32_t)ntp_msg[43]);
            
            // Converte para time_t (segundos desde 1970-01-01)
            // Diferença entre 1900 e 1970 é 2208988800 segundos
            time_t unix_time = (time_t)(ntp_timestamp - 2208988800UL);
            
            // Armazena na variável global
            ntp_response_received = unix_time;
        }
    }
    pbuf_free(p);
}

// =====================================================================
// Sincroniza hora com servidor NTP
bool sync_ntp_time(void) {
    if (!is_wifi_connected()) {
        printf("✗ WiFi não conectado. Use: WIFI ssid password\n");
        return false;
    }
    
    printf("⏳ Sincronizando com NTP (%s)...\n", NTP_SERVER);
    
    ntp_response_received = 0;
    
    // Cria socket UDP para NTP
    ntp_pcb = udp_new();
    if (!ntp_pcb) {
        printf("✗ Erro ao criar socket UDP\n");
        return false;
    }
    
    udp_recv(ntp_pcb, ntp_recv_callback, NULL);
    
    // Monta mensagem NTP
    uint8_t ntp_request[NTP_MSG_LEN] = {0};
    ntp_request[0] = 0x1b;  // LI=0, VN=3, Mode=3 (Client)
    
    // Resolve nome do servidor NTP
    ip_addr_t ntp_server_addr;
    ip4addr_aton(NTP_SERVER, &ntp_server_addr);
    
    // Se for hostname, resolve via DNS
    if (!ip4_addr_isvalid_strict(&ntp_server_addr)) {
        printf("⏳ Resolvendo DNS: %s\n", NTP_SERVER);
        // Simplificado: use IP direto ou implemente DNS
        // Para testes, use um IP direto: 162.159.200.1 (pool.ntp.org)
        ip4addr_aton("162.159.200.1", &ntp_server_addr);
    }
    
    // Envia request NTP
    struct pbuf* p = pbuf_alloc(PBUF_TRANSPORT, NTP_MSG_LEN, PBUF_RAM);
    if (p) {
        memcpy(p->payload, ntp_request, NTP_MSG_LEN);
        udp_sendto(ntp_pcb, p, &ntp_server_addr, NTP_PORT);
        pbuf_free(p);
    }
    
    // Aguarda resposta com timeout
    uint32_t start_time = to_ms_since_boot(get_absolute_time());
    while (!ntp_response_received && 
           (to_ms_since_boot(get_absolute_time()) - start_time) < NTP_TIMEOUT_MS) {
        sleep_ms(10);
        cyw43_arch_poll();
    }
    
    udp_remove(ntp_pcb);
    ntp_pcb = NULL;
    
    if (ntp_response_received) {
        printf("✓ NTP sincronizado com sucesso!\n");
        printf("  Hora Unix: %lu\n", ntp_response_received);
        wifi_config.last_sync = time(NULL);
        return true;
    } else {
        printf("✗ Timeout aguardando resposta NTP\n");
        return false;
    }
}

// =====================================================================
// Inicializa WiFi
bool wifi_init(const char* ssid, const char* password) {
    if (cyw43_arch_init()) {
        printf("✗ Erro ao inicializar WiFi\n");
        return false;
    }
    
    cyw43_arch_enable_sta_mode();
    
    printf("⏳ Conectando ao WiFi: %s\n", ssid);
    
    int err = cyw43_arch_wifi_connect_timeout_ms(ssid, password, CYW43_AUTH_WPA2_MIXED_PSK, 30000);
    
    if (err == 0) {
        printf("✓ WiFi conectado!\n");
        printf("  IP: %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
        
        strncpy(wifi_config.ssid, ssid, sizeof(wifi_config.ssid) - 1);
        strncpy(wifi_config.password, password, sizeof(wifi_config.password) - 1);
        wifi_config.connected = true;
        
        return true;
    } else {
        printf("✗ Erro ao conectar ao WiFi: %d\n", err);
        wifi_config.connected = false;
        return false;
    }
}

// =====================================================================
// Desconecta WiFi
void wifi_disconnect(void) {
    if (wifi_config.connected) {
        cyw43_arch_deinit();
        wifi_config.connected = false;
        printf("✓ WiFi desconectado\n");
    }
}

// =====================================================================
// Verifica se deve sincronizar
bool should_sync_ntp(void) {
    if (!wifi_config.connected) return false;
    
    time_t now = time(NULL);
    return (now - wifi_config.last_sync) >= (wifi_config.sync_interval_ms / 1000);
}

// =====================================================================
// Retorna status WiFi
bool is_wifi_connected(void) {
    return wifi_config.connected && (cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA) == CYW43_LINK_UP);
}

// =====================================================================
// Processa comando WiFi
// Formato: "WIFI ssid password"
void processar_comando_wifi(const char* comando) {
    if (strncmp(comando, "WIFI ", 5) != 0) return;
    
    char buffer[128];
    strncpy(buffer, &comando[5], sizeof(buffer) - 1);
    
    // Parse: "ssid password"
    char* space = strchr(buffer, ' ');
    if (!space) {
        printf("✗ Formato: WIFI ssid password\n");
        return;
    }
    
    *space = '\0';
    char* ssid = buffer;
    char* password = space + 1;
    
    if (wifi_init(ssid, password)) {
        // Sincroniza logo após conectar
        sleep_ms(2000);
        sync_ntp_time();
    }
}

// =====================================================================
// Processa comando de sincronização
void processar_comando_sync(const char* comando) {
    if (strcmp(comando, "SYNC") == 0) {
        sync_ntp_time();
    } else if (strncmp(comando, "SYNCINTERVAL ", 13) == 0) {
        uint32_t interval_min = atoi(&comando[13]);
        if (interval_min > 0 && interval_min <= 1440) {  // 1 minuto a 24 horas
            wifi_config.sync_interval_ms = interval_min * 60 * 1000;
            printf("✓ Intervalo de sincronização: %u minutos\n", interval_min);
        } else {
            printf("✗ Intervalo inválido (1-1440 minutos)\n");
        }
    }
}
