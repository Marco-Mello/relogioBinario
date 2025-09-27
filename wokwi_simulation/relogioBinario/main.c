#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include "ssd1306.h"   // driver SSD1306 (seu ssd1306.h/.c precisam estar no projeto)

#define I2C_RTC i2c0
#define SDA_RTC 4
#define SCL_RTC 5
#define DS1307_ADDR 0x68

#define I2C_OLED i2c1
#define SDA_OLED 2
#define SCL_OLED 3
#define SSD1306_ADDR 0x3C

#define LED_MODE 27   // GP27 indica que o relógio está em modo 12h
#define SWITCH_AMP 28 // GP28 chave ativa modo 12h (0 = 12h, 1 = 24h)

// Pinos separados por grupos (MSB → LSB)
const uint SEGUNDOS_PINS[6] = {21, 22, 23, 24, 25, 26}; // LSB = 26
const uint MINUTOS_PINS[6]  = {12, 13, 14, 15, 16, 17}; // LSB = 17
const uint HORAS_PINS[6]    = {6, 7, 8, 9, 10, 11};     // LSB = 11

ssd1306_t disp;

// =====================================================================
// Conversões BCD
uint8_t bcd_to_dec(uint8_t val) {
    return (val >> 4) * 10 + (val & 0x0F);
}
uint8_t dec_to_bcd(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}

// =====================================================================
// Mostra valor em LEDs + printa no terminal
void mostrar_binario(int valor, const uint *pinos, const char *label) {
    printf("%s: %02d -> ", label, valor);
    for (int i = 5; i >= 0; i--) {
        int bit = (valor >> i) & 1;
        gpio_put(pinos[5 - i], bit);
        printf("%d", bit);
    }
    printf("\n");
}

// =====================================================================
// Ajusta hora no DS1307 (executar uma vez p/ setar)
void ajustar_hora(uint8_t hora, uint8_t minuto, uint8_t segundo) {
    uint8_t buf[4];
    buf[0] = 0x00;
    buf[1] = dec_to_bcd(segundo);
    buf[2] = dec_to_bcd(minuto);
    buf[3] = dec_to_bcd(hora);
    i2c_write_blocking(I2C_RTC, DS1307_ADDR, buf, 4, false);
}

// =====================================================================
// Lê hora atual do DS1307
void ler_hora(int *hora, int *minuto, int *segundo) {
    uint8_t buf[3];
    uint8_t reg = 0x00;
    i2c_write_blocking(I2C_RTC, DS1307_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_RTC, DS1307_ADDR, buf, 3, false);

    *segundo = bcd_to_dec(buf[0] & 0x7F);
    *minuto  = bcd_to_dec(buf[1]);
    *hora    = bcd_to_dec(buf[2] & 0x3F); // 24h
}

// =====================================================================
// Inicializa OLED
void oled_init() {
    i2c_init(I2C_OLED, 400 * 1000);
    gpio_set_function(SDA_OLED, GPIO_FUNC_I2C);
    gpio_set_function(SCL_OLED, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_OLED);
    gpio_pull_up(SCL_OLED);

    ssd1306_init(&disp, 128, 64, false, SSD1306_ADDR, I2C_OLED);
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 0, 0, 1, "Relogio Binario");
    ssd1306_show(&disp);
}

// =====================================================================
// Mostra hora no OLED
void oled_mostrar_hora(int h, int m, int s, bool modo_ampm) {
    char buf[20];
    ssd1306_clear(&disp);

    if (modo_ampm) {
        int hora = h;
        const char *periodo = "AM";
        if (h == 0) { hora = 12; periodo = "AM"; }
        else if (h < 12) { hora = h; periodo = "AM"; }
        else if (h == 12) { hora = 12; periodo = "PM"; }
        else { hora = h - 12; periodo = "PM"; }
        sprintf(buf, "%02d:%02d:%02d %s", hora, m, s, periodo);
    } else {
        sprintf(buf, "%02d:%02d:%02d", h, m, s);
    }

    ssd1306_draw_string(&disp, 10, 30, 2, buf);
    ssd1306_show(&disp);
}

// =====================================================================
// MAIN
int main() {
    stdio_init_all();

    // Inicializa LEDs binários
    for (int i = 0; i < 6; i++) {
        gpio_init(SEGUNDOS_PINS[i]);
        gpio_set_dir(SEGUNDOS_PINS[i], true);

        gpio_init(MINUTOS_PINS[i]);
        gpio_set_dir(MINUTOS_PINS[i], true);

        gpio_init(HORAS_PINS[i]);
        gpio_set_dir(HORAS_PINS[i], true);
    }

    // Inicializa LED de modo (GP27)
    gpio_init(LED_MODE);
    gpio_set_dir(LED_MODE, true);

    // Inicializa chave GP28
    gpio_init(SWITCH_AMP);
    gpio_set_dir(SWITCH_AMP, false);
    gpio_pull_up(SWITCH_AMP);

    // Inicializa I2C0 (RTC)
    i2c_init(I2C_RTC, 100 * 1000);
    gpio_set_function(SDA_RTC, GPIO_FUNC_I2C);
    gpio_set_function(SCL_RTC, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_RTC);
    gpio_pull_up(SCL_RTC);

    // Inicializa OLED (I2C1)
    oled_init();

    printf("Relogio binario iniciado...\n");

    // ⚠️ Ajusta hora inicial em 13h00m00s (use uma vez só)
    ajustar_hora(13, 0, 0);

    while (true) {
        int h, m, s;
        ler_hora(&h, &m, &s);

        bool modo_ampm = (gpio_get(SWITCH_AMP) == 0);

        printf("\n============================\n");

        if (modo_ampm) {
            gpio_put(LED_MODE, 1); // LED aceso em modo 12h

            int hora_mostrar = h;
            const char *periodo = "AM";

            if (h == 0) { hora_mostrar = 12; periodo = "AM"; }
            else if (h < 12) { hora_mostrar = h; periodo = "AM"; }
            else if (h == 12) { hora_mostrar = 12; periodo = "PM"; }
            else { hora_mostrar = h - 12; periodo = "PM"; }

            mostrar_binario(hora_mostrar, HORAS_PINS, "Horas");
            mostrar_binario(m, MINUTOS_PINS, "Minutos");
            mostrar_binario(s, SEGUNDOS_PINS,"Segundos");

            printf("Periodo (12h): %s\n", periodo);
            oled_mostrar_hora(h, m, s, true);

        } else {
            gpio_put(LED_MODE, 0); // LED apagado em modo 24h

            mostrar_binario(h, HORAS_PINS, "Horas");
            mostrar_binario(m, MINUTOS_PINS, "Minutos");
            mostrar_binario(s, SEGUNDOS_PINS,"Segundos");

            printf("Periodo (24h): %s\n", (h >= 12) ? "PM" : "AM");
            oled_mostrar_hora(h, m, s, false);
        }

        sleep_ms(1000);
    }
}
