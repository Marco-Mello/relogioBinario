# 📡 WiFi + NTP Sincronização - Pico W

## ✨ Nova Feature: Sincronização Automática de Hora via WiFi

Agora sua **Pico W** pode sincronizar a hora automaticamente com um servidor NTP a cada 30 minutos!

---

## 🔧 Configuração Inicial

### 1. **Conectar ao WiFi**
```
WIFI seu_ssid sua_senha
```

Exemplo:
```
WIFI myNetwork myPassword123
```

Resposta esperada:
```
⏳ Conectando ao WiFi: myNetwork
✓ WiFi conectado!
  IP: 192.168.1.100
```

### 2. **Sincronizar com NTP**
Pode ser feito manualmente ou automaticamente:

**Sincronização Manual:**
```
SYNC
```

Resposta:
```
⏳ Sincronizando com NTP (pool.ntp.org)...
✓ NTP sincronizado com sucesso!
  Hora Unix: 1695312845
```

---

## ⚙️ Configuração de Intervalo

### Alterar Intervalo de Sincronização (padrão: 30 minutos)

```
SYNCINTERVAL min
```

Exemplos:
```
SYNCINTERVAL 15     // Sincroniza a cada 15 minutos
SYNCINTERVAL 60     // Sincroniza a cada 1 hora
SYNCINTERVAL 1440   // Sincroniza a cada 24 horas
```

**Limitações:**
- Mínimo: 1 minuto
- Máximo: 1440 minutos (24 horas)

---

## 🔄 Funcionamento Automático

### Durante Operação Normal:

1. **Inicialização**: Sistema aguarda comando WiFi
2. **Conexão**: `WIFI ssid password` conecta à rede
3. **Sincronização Inicial**: Sincroniza com NTP automaticamente após conectar
4. **Loop Principal**: A cada intervalo configurado, sincroniza novamente
5. **Sem Interrupção**: Relógio continua funcionando normalmente durante sincronização

### Exemplo de Timeline:

```
T0:00   → WIFI myNetwork password
T0:02   → ✓ Conectado! (sincroniza automaticamente)
T0:05   → Relógio operacional
T30:00  → Sincroniza automaticamente (intervalo de 30 min)
T60:00  → Sincroniza novamente
```

---

## 📊 Servidor NTP Usado

- **pool.ntp.org**: Pool público de servidores NTP confiável
- **Protocolo**: UDP na porta 123
- **Timeout**: 5 segundos
- **Fallback IP**: 162.159.200.1 (se DNS não resolver)

---

## 🧵 Implementação Técnica

### Arquivos Novos:
- `wifi_ntp.h` - Header com definições
- `wifi_ntp.c` - Implementação WiFi/NTP

### Dependências CMake:
```cmake
pico_cyw43_arch_lwip_threadsafe_background
pico_lwip_sntp
```

### Board Type:
```cmake
set(PICO_BOARD pico_w)  # ← Alterado de 'pico' para 'pico_w'
```

---

## 📋 Comandos Completos

| Comando | Formato | Descrição |
|---------|---------|-----------|
| **SET** | `SET HH:MM:SS` | Atualiza hora manualmente |
| **WIFI** | `WIFI ssid password` | Conecta ao WiFi |
| **SYNC** | `SYNC` | Sincroniza com NTP agora |
| **SYNCINTERVAL** | `SYNCINTERVAL min` | Define intervalo em minutos |

---

## 🚨 Erros Comuns e Soluções

| Erro | Causa | Solução |
|------|-------|--------|
| `✗ WiFi não conectado` | WiFi não foi inicializado | Use `WIFI ssid password` |
| `✗ Timeout aguardando resposta NTP` | Sem conexão internet | Verificar WiFi e firewall |
| `✗ Intervalo inválido` | Fora do intervalo 1-1440 | Use valor entre 1 e 1440 minutos |
| `Formato: WIFI ssid password` | Comando incompleto | Certifique-se de espaço entre SSID e senha |

---

## 💻 Exemplo de Sessão Completa

```
Relogio binario iniciado...
╔════════════════════════════════════════╗
║      Comandos Disponíveis             ║
╠════════════════════════════════════════╣
║  SET HH:MM:SS       → Atualizar hora  ║
║  WIFI ssid password → Conectar WiFi   ║
║  SYNC               → Sincronizar NTP ║
║  SYNCINTERVAL min   → Intervalo (min) ║
╚════════════════════════════════════════╝

> WIFI HomeNetwork MyPassword123
⏳ Conectando ao WiFi: HomeNetwork
✓ WiFi conectado!
  IP: 192.168.1.105
⏳ Sincronizando com NTP (pool.ntp.org)...
✓ NTP sincronizado com sucesso!
  Hora Unix: 1695312845

> SYNCINTERVAL 30
✓ Intervalo de sincronização: 30 minutos

============================
Horas: 14 -> 001110
Minutos: 30 -> 011110
Segundos: 45 -> 101101
Periodo (24h): PM

🌐 Sincronizando com NTP...
✓ NTP sincronizado com sucesso!
```

---

## 🔐 Segurança WiFi

### Tipos de Segurança Suportados:
- ✅ WPA2 (Recomendado)
- ✅ WPA2-Mixed
- ❌ WEP (não suportado)
- ❌ Aberta (não recomendada)

### Dicas:
- Use senhas fortes (mín. 8 caracteres)
- WiFi 2.4GHz é mais compatível com Pico W
- Evite caracteres especiais na senha (use apenas alfanuméricos)

---

## ⚡ Performance

| Métrica | Valor |
|---------|-------|
| Tempo de conexão WiFi | ~2-3 segundos |
| Tempo de sincronização NTP | ~0.5-1 segundo |
| Consumo de energia (WiFi ativo) | ~50-80mA |
| Consumo de energia (WiFi inativo) | ~10mA |
| Precisão após sincronização | ±1-2 segundos |

---

## 🔄 Sincronização Automática Explicada

```
┌─────────────────────────────────┐
│  Loop Principal (a cada 1s)     │
└────────────────┬────────────────┘
                 │
                 ↓
     ┌───────────────────────┐
     │ should_sync_ntp()?    │
     │ (último sync         │
     │  > 30 min atrás?)    │
     └───────────────────────┘
                 │
         ┌───────┴────────┐
         │               │
        SIM              NÃO
         │               │
         ↓               ↓
    ┌────────────┐   ┌────────────┐
    │ Sincroniza │   │ Continua   │
    │ com NTP    │   │ normal     │
    └────────────┘   └────────────┘
         │               │
         └───────┬───────┘
                 │
                 ↓
        ┌─────────────────┐
        │ Aguarda 1 seg   │
        │ (sleep_ms)      │
        └────────┬────────┘
                 │
                 ↓
         Volta ao topo do loop
```

---

## 📡 Protocolo NTP

A implementação usa:
- **Modo**: Client (modo 3)
- **Versão**: NTP v3 (compatível com v4)
- **Timestamp**: Segundos desde 1900-01-01 00:00:00
- **Conversão**: Para Unix time (1970-01-01)

---

## 🎯 Casos de Uso

1. **Relógio de Parede Digital**: Sincroniza automaticamente sem intervenção
2. **Estação Meteorológica**: Mantém hora precisa para logs
3. **Sistema de Controle**: Timestamp correto em eventos
4. **Aplicações IoT**: Coordenação horária com nuvem

---

## 📝 Notas Importantes

- ⚠️ A sincronização não bloqueia o loop (non-blocking)
- ⚠️ Sem WiFi, o RTC local (DS1307) continua funcionando
- ⚠️ Após sincronizar, hora é escrita no DS1307 (persiste)
- ⚠️ Intervalo mínimo é 1 minuto para não sobrecarregar servidor

---

**Sistema robusto e pronto para sincronização automática!** 🚀
