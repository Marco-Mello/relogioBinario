# ⏰ Relógio Binário com Raspberry Pi Pico

Um projeto **DIY** para criar um relógio binário usando **Raspberry Pi Pico**, **RTC DS1307** e um **OLED SSD1306**.  
Com direito a LEDs piscando, hora certinha e display OLED para deixar tudo mais elegante. 😍

---

## ✨ Features
- 🟢 Mostra **horas, minutos e segundos** em **binário** com LEDs  
- 🔄 Alterna entre **formato 24h** e **12h (AM/PM)** com uma chave no **GP28**  
- 💡 LED no **GP27** indica quando o modo **12h** está ativo  
- ⏱️ Relógio de tempo real (RTC) **DS1307** para manter a hora mesmo sem energia  
- 🖥️ Tela **OLED SSD1306** exibe a hora em texto (grande e bonito 😎)  

---

## 🛠️ Hardware
- Raspberry Pi Pico  
- Módulo RTC **DS1307** + bateria CR2032  
- Display OLED **SSD1306 (128x64, I²C)**  
- LEDs + resistores para horas/minutos/segundos  
- 1 chave/toggle switch (GP28 → GND)  

---

## ⚡ Ligações

### 🕒 RTC DS1307 (I²C0 - GP4/GP5)
- SDA → GP4  
- SCL → GP5  
- VCC → 3V3  
- GND → GND  

### 🖥️ OLED SSD1306 (I²C1 - GP2/GP3)
- SDA → GP2  
- SCL → GP3  
- VCC → 3V3  
- GND → GND  

### 💡 LEDs Binários
- **Segundos** → GP21..GP26  
- **Minutos** → GP12..GP17  
- **Horas** → GP6..GP11  

### 🔘 Controle
- GP27 → LED indicador de modo 12h  
- GP28 → Chave seletora (0 = AM/PM, 1 = 24h)  

---

## 🖼️ Montagem & 🎮 Demonstração

![Montagem e Demo](./docs/demo.gif)  

> ✨ O GIF acima mostra a protoboard montada com o Raspberry Pi Pico, o módulo RTC DS1307, o OLED SSD1306 e os LEDs funcionando em tempo real.

---

## 🧑‍💻 Simulação no Wokwi

Este projeto também pode ser testado **online no Wokwi** 🎉  

👉 [Abrir simulação no Wokwi](https://wokwi.com/projects/443259630841226241)

Além disso, todos os arquivos da simulação estão disponíveis neste repositório:  

```
wokwi_simulation/relogioBinario/
```

Basta abrir a pasta no [Wokwi](https://wokwi.com/) e rodar a simulação direto no navegador 🚀

---

## 🚀 Como rodar
1. Clone este repositório:
   ```bash
   git clone https://github.com/seu-usuario/relogio-binario.git
   cd relogio-binario
   ```
2. Configure o **Pico SDK** e compile:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```
3. Grave o `.uf2` no Pico (modo BOOTSEL).  
4. 🎉 Pronto, o relógio começa a rodar!  

---

## 📜 Licença
MIT License © 2025 — feito com 💜 e café ☕  
