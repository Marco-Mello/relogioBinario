#ifndef SSD1306_H
#define SSD1306_H

#include "hardware/i2c.h"
#include <stdbool.h>
#include <string.h>

typedef struct {
    int width;
    int height;
    int pages;
    bool external_vcc;
    uint8_t address;
    i2c_inst_t *i2c;
    uint8_t buffer[1024]; // 128x64 = 1024 bytes
} ssd1306_t;

void ssd1306_init(ssd1306_t *ssd, int width, int height, bool external_vcc, uint8_t address, i2c_inst_t *i2c);
void ssd1306_clear(ssd1306_t *ssd);
void ssd1306_show(ssd1306_t *ssd);
void ssd1306_draw_pixel(ssd1306_t *ssd, int x, int y, bool color);
void ssd1306_draw_string(ssd1306_t *ssd, int x, int y, int scale, const char *str);

#endif
