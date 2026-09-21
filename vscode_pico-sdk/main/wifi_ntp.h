#ifndef WIFI_NTP_H
#define WIFI_NTP_H

#include <time.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    char ssid[32];
    char password[64];
    bool connected;
    time_t last_sync;
    uint32_t sync_interval_ms;  // Intervalo de sincronização em ms
} wifi_config_t;

extern wifi_config_t wifi_config;

// Inicializa WiFi com SSID e senha
bool wifi_init(const char* ssid, const char* password);

// Desconecta do WiFi
void wifi_disconnect(void);

// Sincroniza hora com servidor NTP
// Retorna true se sucesso, false se falha
bool sync_ntp_time(void);

// Verifica se é hora de sincronizar novamente
bool should_sync_ntp(void);

// Retorna status da conexão WiFi
bool is_wifi_connected(void);

// Processa comando WiFi via serial
// Formato: "WIFI ssid password"
void processar_comando_wifi(const char* comando);

// Processa comando de sincronização
// Formato: "SYNC" (sincroniza agora)
// Formato: "SYNCINTERVAL ms" (configura intervalo)
void processar_comando_sync(const char* comando);

#endif
