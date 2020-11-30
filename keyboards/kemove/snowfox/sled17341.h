#pragma once

#include <stdint.h>
extern uint8_t led_matrix[288];

extern const uint8_t led_map[61];

typedef struct RgbColor
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
} RgbColor;

typedef struct HsvColor
{
    unsigned char h;
    unsigned char s;
    unsigned char v;
} HsvColor;


void led_init(void);
void led_update_matrix(void);
void led_on(void);
void led_off(void);
static void led_set_color(uint8_t key, uint8_t r, uint8_t g, uint8_t b) {
    led_matrix[led_map[key]] = r;
    led_matrix[led_map[key]+12] = g;
    led_matrix[led_map[key]+24] = b;
}

RgbColor HsvToRgb(HsvColor hsv);
