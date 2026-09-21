# Script de Instalação dos Requisitos - Pico W
# Execute como Administrador!

Write-Host "╔════════════════════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "║   Instalador de Requisitos - Pico W SDK               ║" -ForegroundColor Green
Write-Host "╚════════════════════════════════════════════════════════╝" -ForegroundColor Green
Write-Host ""

# Verificar se é administrador
$admin = [bool]([Security.Principal.WindowsIdentity]::GetCurrent().Groups -match "S-1-5-32-544")
if (-not $admin) {
    Write-Host "❌ ERRO: Este script precisa ser executado como ADMINISTRADOR!" -ForegroundColor Red
    Write-Host "Clique com direito em PowerShell e selecione 'Executar como administrador'" -ForegroundColor Yellow
    exit 1
}

Write-Host "✅ Executando com permissões de administrador" -ForegroundColor Green
Write-Host ""

# Verificar CMake
Write-Host "📦 Verificando CMake..." -ForegroundColor Cyan
$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if ($cmake) {
    Write-Host "✅ CMake já instalado: $($cmake.Source)" -ForegroundColor Green
} else {
    Write-Host "❌ CMake não encontrado. Instale de: https://cmake.org/download/" -ForegroundColor Red
    Write-Host "   Ou use: choco install cmake -y (se tiver Chocolatey)" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Abrindo página de download..." -ForegroundColor Yellow
    Start-Process "https://cmake.org/download/"
}

Write-Host ""

# Verificar GIT
Write-Host "📦 Verificando Git..." -ForegroundColor Cyan
$git = Get-Command git -ErrorAction SilentlyContinue
if ($git) {
    Write-Host "✅ Git já instalado: $($git.Source)" -ForegroundColor Green
} else {
    Write-Host "❌ Git não encontrado. Instale de: https://git-scm.com/download/win" -ForegroundColor Red
    Write-Host "Abrindo página de download..." -ForegroundColor Yellow
    Start-Process "https://git-scm.com/download/win"
}

Write-Host ""

# Verificar ARM GCC
Write-Host "📦 Verificando ARM GCC..." -ForegroundColor Cyan
$armgcc = Get-Command arm-none-eabi-gcc -ErrorAction SilentlyContinue
if ($armgcc) {
    Write-Host "✅ ARM GCC já instalado: $($armgcc.Source)" -ForegroundColor Green
} else {
    Write-Host "❌ ARM GCC não encontrado" -ForegroundColor Red
    Write-Host "   Instale de: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "Abrindo página de download..." -ForegroundColor Yellow
    Start-Process "https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads"
}

Write-Host ""

# Verificar Pico SDK
Write-Host "📦 Verificando Pico SDK..." -ForegroundColor Cyan
$picoSdk = "C:\Users\$env:USERNAME\pico-sdk"
if (Test-Path $picoSdk) {
    Write-Host "✅ Pico SDK encontrado: $picoSdk" -ForegroundColor Green
} else {
    Write-Host "❌ Pico SDK não encontrado" -ForegroundColor Red
    Write-Host "   Clone com: git clone https://github.com/raspberrypi/pico-sdk.git $picoSdk" -ForegroundColor Yellow
}

Write-Host ""

# Verificar variáveis de ambiente
Write-Host "🔧 Verificando Variáveis de Ambiente..." -ForegroundColor Cyan
$picoSdkPath = $env:PICO_SDK_PATH
$picoExtrasPath = $env:PICO_EXTRAS_PATH

if ($picoSdkPath) {
    Write-Host "✅ PICO_SDK_PATH: $picoSdkPath" -ForegroundColor Green
} else {
    Write-Host "❌ PICO_SDK_PATH não definida" -ForegroundColor Red
    Write-Host "   Execute: setx PICO_SDK_PATH 'C:\Users\$env:USERNAME\pico-sdk'" -ForegroundColor Yellow
}

if ($picoExtrasPath) {
    Write-Host "✅ PICO_EXTRAS_PATH: $picoExtrasPath" -ForegroundColor Green
} else {
    Write-Host "❌ PICO_EXTRAS_PATH não definida" -ForegroundColor Red
    Write-Host "   Execute: setx PICO_EXTRAS_PATH 'C:\Users\$env:USERNAME\pico-extras'" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "╔════════════════════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "║           Verificação Concluída                        ║" -ForegroundColor Green
Write-Host "╚════════════════════════════════════════════════════════╝" -ForegroundColor Green
Write-Host ""
Write-Host "📝 Próximos passos:" -ForegroundColor Cyan
Write-Host "  1. Instale qualquer ferramenta faltante (links acima)"
Write-Host "  2. Reinicie o PowerShell após instalar CMake/GCC"
Write-Host "  3. Execute: cd Desktop\relogioBinario\vscode_pico-sdk\build"
Write-Host "  4. Execute: cmake .."
Write-Host "  5. Execute: make -j4"
Write-Host ""
