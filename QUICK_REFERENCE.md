# ⚡ QUICK REFERENCE - Comandos e Configuração

## 🎮 Comandos Serial Rápidos

### Terminal Serial
```
Baud Rate: 115200
Data Bits: 8
Stop Bits: 1
Parity: None
```

---

## 📋 Tabela de Comandos

| Comando | Syntax | Exemplo | Resposta |
|---------|--------|---------|----------|
| **SET** | `SET HH:MM:SS` | `SET 14:30:45` | `✓ Hora atualizada para 14:30:45` |
| **WIFI** | `WIFI ssid password` | `WIFI MyWifi Pass123` | `✓ WiFi conectado!` |
| **SYNC** | `SYNC` | `SYNC` | `✓ NTP sincronizado com sucesso!` |
| **SYNCINTERVAL** | `SYNCINTERVAL min` | `SYNCINTERVAL 30` | `✓ Intervalo: 30 minutos` |

---

## ✅ Validações

### SET HH:MM:SS
- Horas: 0-23 ✓
- Minutos: 0-59 ✓
- Segundos: 0-59 ✓

### WIFI ssid password
- SSID: até 32 caracteres
- Senha: até 64 caracteres
- Segurança: WPA2 (recomendado)

### SYNCINTERVAL
- Mínimo: 1 minuto
- Máximo: 1440 minutos (24h)
- Padrão: 30 minutos

---

## 🔴 Erros Comuns

| Erro | Causa | Solução |
|------|-------|---------|
| `✗ Valores inválidos` | Fora do intervalo | Use 0-23 horas, 0-59 min/seg |
| `✗ Formato inválido` | Comando errado | Use `SET HH:MM:SS` exato |
| `✗ WiFi não conectado` | WiFi não iniciado | Use `WIFI ssid password` |
| `✗ Timeout NTP` | Sem internet | Verifique conexão WiFi |

---

## 📊 Pinagem Pico W

### GPIO Usados
```
I²C0 (RTC):
├─ GP4 → SDA (RTC)
└─ GP5 → SCL (RTC)

I²C1 (OLED):
├─ GP2 → SDA (OLED)
└─ GP3 → SCL (OLED)

LEDs:
├─ GP6-11  → Horas (6 bits)
├─ GP12-17 → Minutos (6 bits)
├─ GP21-26 → Segundos (6 bits)
├─ GP27    → LED Modo 12h
└─ GP28    → Switch 24h/12h
```

---

## 🚀 Compilação Rápida

```bash
cd vscode_pico-sdk
rm -rf build && mkdir build && cd build
cmake ..
make -j4
# Saída: pico_emb.uf2
```

---

## 📁 Arquivos Importantes

### Código
```
vscode_pico-sdk/main/
├── main.c ..................... Lógica principal
├── wifi_ntp.c ................. WiFi + NTP (novo)
├── wifi_ntp.h ................. Headers (novo)
├── ssd1306.c .................. Driver OLED
└── CMakeLists.txt ............. Configuração build
```

### Documentação
```
├── EXECUTIVE_SUMMARY.md ....... Este arquivo
├── UPDATES_README.md .......... O que mudou
├── SERIAL_COMMANDS.md ......... Guia SET
├── WIFI_NTP.md ................ Guia WiFi/NTP
├── COMPILATION.md ............ Como compilar
└── TEST_EXAMPLES.md .......... Exemplos teste
```

---

## 🧪 Teste Rápido

```bash
# Terminal Serial (115200)

# 1. Testar SET
> SET 12:00:00
✓ Hora atualizada para 12:00:00

# 2. Testar WiFi
> WIFI MeuWiFi MinhaSenh@
⏳ Conectando ao WiFi: MeuWiFi
✓ WiFi conectado!
  IP: 192.168.x.x

# 3. Testar NTP
> SYNC
⏳ Sincronizando com NTP...
✓ NTP sincronizado com sucesso!
  Hora Unix: 1695312845

# ✓ Pronto!
```

---

## 🔄 Fluxo de Uso Típico

```
1. Compilar e gravar .uf2
   ↓
2. Abrir terminal serial (115200)
   ↓
3. Conectar WiFi: WIFI ssid password
   ↓
4. (Automático) Sincroniza com NTP
   ↓
5. Hora exibida em LEDs + OLED
   ↓
6. A cada 30 min: resincroniza
```

---

## 📊 Performance

| Operação | Tempo | Bloqueante? |
|----------|-------|------------|
| Conexão WiFi | 2-3s | Sim |
| Sincronização NTP | 0.5-1s | Não |
| Loop principal | 1s | Sim |
| Comando SET | <100ms | Não |
| Atualização OLED | <50ms | Não |

---

## ⚙️ Configuração Padrão

```c
#define NTP_SERVER "pool.ntp.org"      // Servidor NTP
#define NTP_PORT 123                   // Porta UDP
#define NTP_TIMEOUT_MS 5000            // 5 segundos
#define SYNCINTERVAL 30*60*1000        // 30 minutos
#define BAUD_RATE 115200               // Serial
#define I2C_FREQ_RTC 100*1000          // 100 kHz
#define I2C_FREQ_OLED 400*1000         // 400 kHz
```

---

## 🔐 Segurança WiFi

### Recomendado
- ✅ WPA2 / WPA2-PSK
- ✅ Senha forte (8+ caracteres)
- ✅ Alfanuméricos + símbolos

### Não Recomendado
- ❌ WEP (não suportado)
- ❌ WiFi aberto (sem senha)
- ❌ Caracteres especiais na senha

---

## 🐛 Debug

### Verificar Conexão WiFi
```
> WIFI seu_wifi sua_senha
✓ WiFi conectado!
  IP: 192.168.x.x
  Status: CYW43_LINK_UP
```

### Verificar NTP
```
> SYNC
✓ NTP sincronizado com sucesso!
  Hora Unix: 1695312845
  Último sync: agora
```

### Verificar Hora RTC
```
Terminal mostra:
============================
Horas: 14 -> 001110
Minutos: 30 -> 011110
Segundos: 45 -> 101101
Periodo (24h): PM
```

---

## 📱 Compatibilidade

| Hardware | Feature 1 | Feature 2 |
|----------|-----------|-----------|
| **Pico** | ✅ Sim | ❌ Não (sem WiFi) |
| **Pico W** | ✅ Sim | ✅ Sim |
| **Pico 2** | ✅ Sim | ? (não testado) |

---

## 🎓 Referências Rápidas

### Serial
- SET: `SERIAL_COMMANDS.md`
- Exemplos: `TEST_EXAMPLES.md`

### WiFi/NTP
- Guia: `WIFI_NTP.md`
- Técnico: `WIFI_NTP_SUMMARY.md`
- Compilação: `COMPILATION.md`

### Geral
- Índice: `DOCUMENTATION_INDEX.md`
- Resumo: `EXECUTIVE_SUMMARY.md`

---

## 💾 Salvando Configuração

### Persistência
- RTC + Bateria CR2032 → Hora persiste sem energia ✓
- Credenciais WiFi → Não salvas (use comando sempre) ⚠️

### Como Restaurar
```
1. Reconectar WiFi: WIFI ssid password
2. Sincronizar: SYNC
3. Pronto!
```

---

## 🔧 Dicas Pro

1. **Autocompletar:** Alguns terminais suportam histórico (⬆️/⬇️)
2. **Copiar-colar:** Cole diretamente no terminal
3. **Macros:** Configure macros em PuTTY/Minicom
4. **Log:** Salve saída do terminal para debug

---

## 📞 Suporte Rápido

**Q: Não aparece menu?**  
A: Verifique baud rate (115200) e porta USB

**Q: WiFi não conecta?**  
A: Verifique SSID/senha e WPA2 habilitado

**Q: NTP timeout?**  
A: Verifique internet e firewall (UDP 123)

**Q: Hora não atualiza?**  
A: Verifique conexão I²C RTC

---

## 🎯 Checklist de Deploy

- [ ] Compilou sem erros
- [ ] Gravou no Pico W
- [ ] Terminal serial conectado
- [ ] Menu aparece ao iniciar
- [ ] SET funciona
- [ ] WiFi conecta
- [ ] NTP sincroniza
- [ ] LEDs atualizam
- [ ] OLED mostra hora

---

## 🏁 Pronto Para Começar?

1. Ler [EXECUTIVE_SUMMARY.md](EXECUTIVE_SUMMARY.md) (5 min)
2. Compilar com [COMPILATION.md](COMPILATION.md) (5 min)
3. Testar com [TEST_EXAMPLES.md](TEST_EXAMPLES.md) (5 min)

**Total: 15 minutos para estar operacional! ⚡**

---

**Desenvolvido para Raspberry Pi Pico W ⏰**

*Leia DOCUMENTATION_INDEX.md para navegação completa*
