# 🔧 Guia de Compilação - WiFi + NTP Habilitado

## ⚠️ Pré-requisitos

### Hardware:
- ✅ **Raspberry Pi Pico W** (não funciona em Pico normal)
- ✅ RTC DS1307
- ✅ OLED SSD1306
- ✅ LEDs e resistores (como antes)

### Software:
- Pico SDK 2.0+
- CMake 3.12+
- Compilador ARM GCC
- VS Code + Extensão Pico (opcional)

---

## 🚀 Compilação no Linux/Mac

```bash
# 1. Clone o repositório
git clone https://github.com/Marco-Mello/relogioBinario.git
cd relogioBinario

# 2. Entre na pasta do projeto
cd vscode_pico-sdk

# 3. Crie pasta de build
mkdir build
cd build

# 4. Configure com CMake
cmake ..

# 5. Compile
make -j4

# 6. O arquivo .uf2 estará em:
# build/pico_emb.uf2
```

---

## 🪟 Compilação no Windows

### Opção 1: VS Code Extension

1. Abra a pasta `vscode_pico-sdk` no VS Code
2. Instale a extensão "Raspberry Pi Pico"
3. Aperte `Ctrl+Shift+P` → "Pico: Create New Project..."
4. Ou use o terminal integrado:

```powershell
cd vscode_pico-sdk
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make -j4
```

### Opção 2: Terminal PowerShell

```powershell
# Assumindo que Pico SDK está instalado e em %PATH%

cd vscode_pico-sdk
mkdir build
cd build

# Configure
cmake -G "MinGW Makefiles" ..

# Compile
mingw32-make -j4

# Saída: pico_emb.uf2
```

---

## 📥 Gravação no Pico W

### Método 1: Via USB BOOTSEL

1. **Pressione BOOTSEL** (botão no Pico) enquanto conecta USB
2. Aparecerá um drive chamado `RPI-RP2`
3. **Copie** `pico_emb.uf2` para o drive
4. Pico reinicia automaticamente e executa o código

### Método 2: Via picotool

```bash
# Conecte Pico em modo BOOTSEL
picotool load pico_emb.uf2 -v
```

### Método 3: VS Code

1. No VS Code: `Ctrl+Shift+P`
2. Selecione "Pico: Run Project"
3. Selecione porta USB
4. Código é compilado e gravado automaticamente

---

## ✅ Verificar Instalação

Após gravar, abra terminal serial (115200 baud):

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
```

Se ver isso, está funcionando! ✅

---

## 🐛 Troubleshooting Compilação

### Erro: `pico_cyw43_arch_lwip_threadsafe_background not found`

**Causa**: Pico SDK muito antigo
**Solução**: Atualize para SDK 2.0+

```bash
# Verifique versão
git -C $PICO_SDK_PATH show-ref --head | head -1
```

### Erro: `PICO_BOARD pico_w not found`

**Causa**: Board type não reconhecido
**Solução**: Limpe cache e reconfigure

```bash
rm -rf build
mkdir build
cd build
cmake ..
```

### Erro: `-lwip` não encontrado

**Causa**: lwIP (networking) não incluído
**Solução**: Certifique-se que `pico_extras_import_optional.cmake` está incluído

No `CMakeLists.txt`:
```cmake
include(pico_extras_import_optional.cmake)
```

---

## 📊 Tamanho do Binário

| Componente | Tamanho |
|-----------|---------|
| Código base (sem WiFi) | ~50 KB |
| WiFi + NTP | ~+120 KB |
| **Total** | **~170 KB** |

Pico W tem 2MB de flash, então não há problema de espaço. ✅

---

## 🔌 Pinagem Necessária (igual antes)

| Componente | Pino GPIO | Notas |
|-----------|-----------|-------|
| RTC SDA | GP4 | I²C0 |
| RTC SCL | GP5 | I²C0 |
| OLED SDA | GP2 | I²C1 |
| OLED SCL | GP3 | I²C1 |
| LEDs Horas | GP6-11 | 6 bits |
| LEDs Minutos | GP12-17 | 6 bits |
| LEDs Segundos | GP21-26 | 6 bits |
| LED Modo | GP27 | Indicador 12h |
| Switch | GP28 | Toggle 24h/12h |

**WiFi interno**: Usa pinos do módulo CYW43 integrado (automático)

---

## 🌐 Requisitos de Rede

### WiFi:
- IEEE 802.11b/g/n
- Frequência: 2.4 GHz (5GHz não suportado)
- Segurança: WPA2/WPA2-PSK
- Velocidade mínima: 1 Mbps é o suficiente

### Internet:
- Conexão para servidor NTP (pool.ntp.org porta 123 UDP)
- Não requer IP público
- Funciona atrás de NAT/Firewall (geralmente)

---

## 📝 Environment Variables (Opcional)

Para compilação automática:

```bash
# Linux/Mac
export PICO_SDK_PATH=/path/to/pico-sdk
export PICO_EXTRAS_PATH=/path/to/pico-extras

# Windows PowerShell
$env:PICO_SDK_PATH = "C:\path\to\pico-sdk"
$env:PICO_EXTRAS_PATH = "C:\path\to\pico-extras"
```

---

## 🧪 Teste Rápido

Após compilar e gravar:

```bash
# Terminal serial (use miniterm, picocom ou Arduino IDE)
miniterm.py /dev/ttyACM0 115200

# No terminal:
> WIFI sua_rede sua_senha
⏳ Conectando ao WiFi: sua_rede
✓ WiFi conectado!
  IP: 192.168.x.x

> SYNC
⏳ Sincronizando com NTP...
✓ NTP sincronizado com sucesso!
```

---

## 📚 Recursos Adicionais

- [Pico W SDK Docs](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [Pico W Datasheet](https://datasheets.raspberrypi.com/picow/pico-w-datasheet.pdf)
- [WiFi Driver](https://github.com/raspberrypi/cyw43-driver)
- [lwIP Documentation](https://lwip.fandom.com/wiki/Main_Page)

---

## ✨ Dicas de Desenvolvimento

1. **Debug via Serial**:
   ```bash
   miniterm.py /dev/ttyACM0 115200 --exit-char=27
   ```

2. **Rebuild Rápido**:
   ```bash
   cd build && make -j4
   ```

3. **Limpeza Completa**:
   ```bash
   rm -rf build && mkdir build && cd build && cmake .. && make
   ```

4. **Logs Persistentes**:
   Redirecione saída serial para arquivo:
   ```bash
   miniterm.py /dev/ttyACM0 115200 > log.txt 2>&1 &
   ```

---

**Sistema compilado e pronto para usar!** 🚀
