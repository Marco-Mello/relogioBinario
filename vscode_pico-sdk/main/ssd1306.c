#include "ssd1306.h"
#include "pico/stdlib.h"

// Fonte mínima 5x7 (ASCII 32-127)
static const uint8_t font[95][5] = {
    {0,0,0,0,0}, {0,0,95,0,0}, {0,7,0,7,0}, {20,127,20,127,20}, {36,42,127,42,18},
    {35,19,8,100,98}, {54,73,85,34,80}, {0,5,3,0,0}, {0,28,34,65,0}, {0,65,34,28,0},
    {20,8,62,8,20}, {8,8,62,8,8}, {0,80,48,0,0}, {8,8,8,8,8}, {0,96,96,0,0}, {32,16,8,4,2},
    {62,81,73,69,62}, {0,66,127,64,0}, {66,97,81,73,70}, {33,65,69,75,49}, {24,20,18,127,16},
    {39,69,69,69,57}, {60,74,73,73,48}, {1,113,9,5,3}, {54,73,73,73,54}, {6,73,73,41,30},
    {0,54,54,0,0}, {0,86,54,0,0}, {8,20,34,65,0}, {20,20,20,20,20}, {0,65,34,20,8},
    {2,1,81,9,6}, {50,73,121,65,62}, {126,9,9,9,126}, {127,73,73,73,54}, {62,65,65,65,34},
    {127,65,65,34,28}, {127,73,73,73,65}, {127,9,9,9,1}, {62,65,73,73,122}, {127,8,8,8,127},
    {0,65,127,65,0}, {32,64,65,63,1}, {127,8,20,34,65}, {127,64,64,64,64}, {127,2,12,2,127},
    {127,4,8,16,127}, {62,65,65,65,62}, {127,9,9,9,6}, {62,65,81,33,94}, {127,9,25,41,70},
    {70,73,73,73,49}, {1,1,127,1,1}, {63,64,64,64,63}, {31,32,64,32,31}, {63,64,48,64,63},
    {99,20,8,20,99}, {7,8,112,8,7}, {97,81,73,69,67}, {0,127,65,65,0}, {2,4,8,16,32},
    {0,65,65,127,0}, {4,2,1,2,4}, {64,64,64,64,64}, {0,3,7,8,0}, {32,84,84,120,64},
    {127,40,68,68,56}, {56,68,68,68,40}, {56,68,68,40,127}, {56,84,84,92,8},
    {8,126,9,1,2}, {12,82,82,60,0}, {127,8,4,4,120}, {0,68,125,64,0}, {32,64,68,61,0},
    {127,16,40,68,0}, {0,65,127,64,0}, {124,4,24,4,120}, {124,8,4,4,120}, {56,68,68,68,56},
    {124,20,20,20,8}, {8,20,20,124,0}, {124,8,4,4,0}, {72,84,84,84,36}, {4,63,68,64,32},
    {60,64,64,32,124}, {28,32,64,32,28}, {60,64,48,64,60}, {68,40,16,40,68}, {12,80,80,124,0},
    {68,100,84,76,68}, {0,8,54,65,0}, {0,0,127,0,0}, {0,65,54,8,0}, {8,4,12,8,4}
};

// Envia comando
static void ssd1306_command(ssd1306_t *ssd, uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    i2c_write_blocking(ssd->i2c, ssd->address, buf, 2, false);
}

void ssd1306_init(ssd1306_t *ssd, int width, int height, bool external_vcc, uint8_t address, i2c_inst_t *i2c) {
    ssd->width = width;
    ssd->height = height;
    ssd->pages = height / 8;
    ssd->external_vcc = external_vcc;
    ssd->address = address;
    ssd->i2c = i2c;
    memset(ssd->buffer, 0, sizeof(ssd->buffer));

    ssd1306_command(ssd, 0xAE); // Display off
    ssd1306_command(ssd, 0x20); // Memory mode
    ssd1306_command(ssd, 0x00); // Horizontal addressing mode
    ssd1306_command(ssd, 0xA1); // Segment remap
    ssd1306_command(ssd, 0xC8); // COM scan dec
    ssd1306_command(ssd, 0xDA); // COM pins
    ssd1306_command(ssd, 0x12);
    ssd1306_command(ssd, 0x81); // Contrast
    ssd1306_command(ssd, 0x7F);
    ssd1306_command(ssd, 0xA4); // Resume RAM
    ssd1306_command(ssd, 0xA6); // Normal display
    ssd1306_command(ssd, 0xAF); // Display on
}

void ssd1306_clear(ssd1306_t *ssd) {
    memset(ssd->buffer, 0, sizeof(ssd->buffer));
}

void ssd1306_show(ssd1306_t *ssd) {
    for (int page = 0; page < ssd->pages; page++) {
        ssd1306_command(ssd, 0xB0 + page);
        ssd1306_command(ssd, 0x00);
        ssd1306_command(ssd, 0x10);
        uint8_t buf[129];
        buf[0] = 0x40;
        memcpy(buf+1, &ssd->buffer[ssd->width * page], ssd->width);
        i2c_write_blocking(ssd->i2c, ssd->address, buf, 129, false);
    }
}

void ssd1306_draw_pixel(ssd1306_t *ssd, int x, int y, bool color) {
    if (x < 0 || x >= ssd->width || y < 0 || y >= ssd->height) return;
    if (color)
        ssd->buffer[x + (y/8)*ssd->width] |= (1 << (y&7));
    else
        ssd->buffer[x + (y/8)*ssd->width] &= ~(1 << (y&7));
}

void ssd1306_draw_char(ssd1306_t *ssd, int x, int y, int scale, char c) {
    if (c < 32 || c > 126) return;
    for (int col = 0; col < 5; col++) {
        uint8_t line = font[c-32][col];
        for (int row = 0; row < 7; row++) {
            if (line & (1 << row)) {
                for (int dx=0; dx<scale; dx++) {
                    for (int dy=0; dy<scale; dy++) {
                        ssd1306_draw_pixel(ssd, x+col*scale+dx, y+row*scale+dy, true);
                    }
                }
            }
        }
    }
}

void ssd1306_draw_string(ssd1306_t *ssd, int x, int y, int scale, const char *str) {
    while (*str) {
        ssd1306_draw_char(ssd, x, y, scale, *str++);
        x += 6*scale;
    }
}
