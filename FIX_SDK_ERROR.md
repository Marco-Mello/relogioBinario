# 🔧 RESOLVER ERRO "Failed to download and install SDK"

## ✅ O QUE JÁ FOI FEITO

- ✅ Pico SDK clonado: `C:\Users\marxuco\pico-sdk`
- ✅ Pico Extras clonado: `C:\Users\marxuco\pico-extras`
- ✅ Variáveis de ambiente configuradas
- ✅ Git verificado

---

## ❌ O QUE ESTÁ FALTANDO

**CMake não está instalado!**

CMake é necessário para compilar o projeto.

---

## 🚀 SOLUÇÃO RÁPIDA (3 passos)

### **Passo 1: Instalar CMake**

#### Opção A: Usando Chocolatey (Mais fácil)
```powershell
# Abra PowerShell como ADMINISTRADOR e execute:
choco install cmake -y
```

#### Opção B: Download Manual
1. Vá em: https://cmake.org/download/
2. Baixe "Windows x86_64 Installer"
3. Execute o instalador
4. ✅ **Marque a opção "Add CMake to system PATH"**
5. Clique Next até terminar

#### Opção C: Usando Windows Package Manager
```powershell
# PowerShell como ADMINISTRADOR:
winget install Kitware.CMake
```

### **Passo 2: Instalar ARM GCC (se não tiver)**

1. Vá em: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
2. Baixe a versão Windows (não a WSL!)
3. Execute o instalador
4. ✅ **Marque "Add path to environment variable"**

### **Passo 3: Reiniciar e Compilar**

```powershell
# Feche completamente e reabra PowerShell como ADMINISTRADOR

# Verifique se está instalado:
cmake --version
arm-none-eabi-gcc --version

# Compile:
cd C:\Users\marxuco\Desktop\relogioBinario\vscode_pico-sdk
mkdir build
cd build
cmake ..
make -j4
```

---

## ✅ VERIFICAÇÃO DE INSTALAÇÃO

Depois de instalar, execute o script que criei:

```powershell
# PowerShell como ADMINISTRADOR
powershell -ExecutionPolicy Bypass -File "C:\Users\marxuco\Desktop\relogioBinario\INSTALL_REQUIREMENTS.ps1"
```

Ele vai verificar tudo automaticamente!

---

## 🎯 SE AINDA NÃO FUNCIONAR

Me mostre a saída de:

```powershell
cmake --version
arm-none-eabi-gcc --version
echo $env:PICO_SDK_PATH
echo $env:PICO_EXTRAS_PATH
```

---

## 📊 CHECKLIST DE INSTALAÇÃO

- [ ] CMake instalado (versão 3.12+)
- [ ] ARM GCC instalado
- [ ] CMake no PATH do sistema
- [ ] ARM GCC no PATH do sistema
- [ ] PICO_SDK_PATH configurada
- [ ] PICO_EXTRAS_PATH configurada
- [ ] PowerShell reiniciado após instalar ferramentas

---

## 🚀 DEPOIS DE TUDO INSTALADO

```bash
cd C:\Users\marxuco\Desktop\relogioBinario\vscode_pico-sdk\build
cmake ..
make -j4
# Aguarde ~30 segundos
# Pronto! pico_emb.uf2 será criado
```

---

**Avise-me quando tiver tudo instalado que eu compilo para você!** 🎉
