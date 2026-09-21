# 🧪 Exemplos de Teste - Serial Commands

## Cenário 1: Teste Básico (Linux/Mac)

```bash
# Conectar via miniterm
miniterm.py /dev/ttyACM0 115200

# Saída inicial:
# Relogio binario iniciado...
# ╔════════════════════════════════╗
# ║      Comandos Disponíveis      ║
# ╠════════════════════════════════╣
# ║  SET HH:MM:SS  → Atualizar    ║
# ║  Ex: SET 14:30:45             ║
# ╚════════════════════════════════╝

# Digitar e pressionar ENTER:
SET 14:30:45

# Resposta esperada:
# ✓ Hora atualizada para 14:30:45
#
# ============================
# Horas: 14 -> 001110
# Minutos: 30 -> 011110
# Segundos: 45 -> 101101
# Periodo (24h): PM
```

---

## Cenário 2: Teste de Validação

```bash
# Teste 1: Valores inválidos
> SET 25:00:00
✗ Valores inválidos! Use: SET HH:MM:SS (00-23:00-59:00-59)

# Teste 2: Formato incorreto
> SET 14-30-45
✗ Formato inválido! Use: SET HH:MM:SS

# Teste 3: Hora meia-noite
> SET 00:00:00
✓ Hora atualizada para 00:00:00

# Teste 4: Hora limite
> SET 23:59:59
✓ Hora atualizada para 23:59:59
```

---

## Cenário 3: Windows (PuTTY)

1. Abrir PuTTY
2. Connection Type: Serial
3. Serial line: COM3 (verificar Device Manager)
4. Speed: 115200
5. Click "Open"

```
Relogio binario iniciado...
╔════════════════════════════════╗
║      Comandos Disponíveis      ║
╠════════════════════════════════╣
║  SET HH:MM:SS  → Atualizar    ║
║  Ex: SET 14:30:45             ║
╚════════════════════════════════╝

[Digitar na janela aberta]
SET 09:30:15

[Resposta]
✓ Hora atualizada para 09:30:15
```

---

## Cenário 4: Arduino IDE Serial Monitor

1. Abrir Arduino IDE
2. Tools → Serial Monitor
3. Selecionar baud rate: 115200
4. Digitar comando e pressionar "Send"

```
--- Inicialização ---
Relogio binario iniciado...
╔════════════════════════════════╗
║      Comandos Disponíveis      ║
╠════════════════════════════════╣
║  SET HH:MM:SS  → Atualizar    ║
║  Ex: SET 14:30:45             ║
╚════════════════════════════════╝

--- Digite no campo de entrada ---
SET 20:45:30

--- Resposta no console ---
✓ Hora atualizada para 20:45:30
```

---

## Cenário 5: Script Python (Automático)

```python
import serial
import time

# Conectar ao Pico
pico = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
time.sleep(2)  # Esperar inicialização

# Enviar comando
pico.write(b'SET 15:30:00\r\n')

# Ler resposta
response = pico.readline().decode()
print(response)  # Saída: ✓ Hora atualizada para 15:30:00

pico.close()
```

---

## Cenário 6: Teste de Modo AM/PM

```bash
# Switch GP28 ligado (pull-up = modo 12h)

> SET 00:15:30
✓ Hora atualizada para 00:15:30

============================
Horas: 12 -> 001100
Minutos: 15 -> 001111
Segundos: 30 -> 011110
Periodo (12h): AM
```

---

## 📋 Checklist de Teste Completo

- [ ] Comando correto atualiza a hora
- [ ] LED indicador (GP27) muda com switch
- [ ] OLED mostra hora atualizada
- [ ] LEDs binários acendem/apagam corretamente
- [ ] Validação rejeita horas > 23
- [ ] Validação rejeita minutos > 59
- [ ] Validação rejeita segundos > 59
- [ ] Formato incorreto mostra erro
- [ ] Menu aparece na inicialização
- [ ] Hora persiste sem energia (RTC com bateria)
- [ ] Terminal mostra debug a cada segundo

---

## 🔍 Troubleshooting

| Problema | Solução |
|----------|---------|
| Sem resposta ao digitar | Verificar baud rate (115200) |
| Caracteres estranhos | Verificar conexão USB |
| Comando não reconhecido | Usar maiúsculas: `SET` |
| Nenhum feedback | Verificar `stdio_usb_connected()` |
| Hora não muda | Verificar conexão I²C do RTC |

---

**Pronto para testar!** 🚀
