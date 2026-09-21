# 📡 Comandos Serial - Relógio Binário

## ✨ Nova Feature: Atualização de Hora via Serial

Agora é possível **atualizar a hora do relógio em tempo real** através da porta serial (USB)!

---

## 🔧 Como Usar

### 1. **Conectar via Serial**
Use um terminal serial (PuTTY, Minicom, Arduino IDE Serial Monitor, etc.):
- **Baud Rate**: 115200
- **Data Bits**: 8
- **Stop Bits**: 1
- **Parity**: None

### 2. **Comando: SET HH:MM:SS**

#### Formato:
```
SET HH:MM:SS
```

#### Exemplos:
```
SET 14:30:45    // Atualiza para 14h30m45s
SET 09:15:00    // Atualiza para 09h15m00s
SET 23:59:59    // Atualiza para 23h59m59s
```

### 3. **Resposta do Sistema**

✅ **Sucesso:**
```
✓ Hora atualizada para 14:30:45
```

❌ **Erro de Formato:**
```
✗ Formato inválido! Use: SET HH:MM:SS
```

❌ **Valores Fora do Intervalo:**
```
✗ Valores inválidos! Use: SET HH:MM:SS (00-23:00-59:00-59)
```

---

## 📋 Validações

O comando valida automaticamente:
- **Horas**: 00-23 (formato 24h)
- **Minutos**: 00-59
- **Segundos**: 00-59

Valores fora desse intervalo são **rejeitados**.

---

## 💡 Dicas

1. **Menu de Ajuda**: Digite qualquer comando inválido para ver as opções disponíveis
2. **Sem Espaços Extras**: Use exatamente `SET HH:MM:SS` (sem espaços adicionais)
3. **Formato com Zeros**: Use 2 dígitos sempre (ex: `SET 09:05:03`, não `SET 9:5:3`)
4. **Persistência**: A hora atualizada é **salva no DS1307** e persiste mesmo sem alimentação

---

## 🔄 Fluxo no Loop Principal

A cada iteração do loop (a cada 1 segundo):
1. ✅ Lê comando serial (se houver)
2. ✅ Valida e processa
3. ✅ Atualiza DS1307 (se válido)
4. ✅ Exibe hora nos LEDs e OLED
5. ✅ Aguarda 1 segundo até próxima leitura

---

## 📝 Implementação Técnica

### Função Principal:
```c
void processar_comando_serial()
```

### Características:
- ⏱️ Timeout de 100ms na leitura
- 🔍 Validação rigorosa de entrada
- 🛡️ Proteção contra buffer overflow
- 📊 Feedback visual (check/cross icons)
- 🔌 Verifica conexão USB antes de processar

---

## 🧪 Testando

### No Wokwi:
1. Abra a simulação
2. Clique em "Serial Monitor"
3. Digite: `SET 14:30:45`
4. Pressione Enter

### No Raspberry Pi Pico Real:
1. Conecte via USB
2. Abra terminal serial (ex: `miniterm.py /dev/ttyACM0 115200`)
3. Digite: `SET 14:30:45`
4. Pressione Enter

---

## ❓ FAQ

**P: Posso usar comandos em letra minúscula?**
R: Não, deve ser `SET` em maiúsculas.

**P: E se não enviar o comando corretamente?**
R: Sistema mostra menu de ajuda com opções válidas.

**P: A hora persiste sem energia?**
R: Sim! O DS1307 tem bateria (CR2032) integrada.

**P: Posso atualizar a hora sem parar o relógio?**
R: Sim! Funciona em tempo real, sem reinicialização.

---

## 🔄 Exemplo de Sessão

```
Relogio binario iniciado...
╔════════════════════════════════╗
║      Comandos Disponíveis      ║
╠════════════════════════════════╣
║  SET HH:MM:SS  → Atualizar    ║
║  Ex: SET 14:30:45             ║
╚════════════════════════════════╝

[Usuário digita: SET 14:30:45]

✓ Hora atualizada para 14:30:45

============================
Horas: 14 -> 001110
Minutos: 30 -> 011110
Segundos: 45 -> 101101
Periodo (24h): PM
```

---

**Desenvolvido para facilitar a sincronização do relógio! ⏰**
