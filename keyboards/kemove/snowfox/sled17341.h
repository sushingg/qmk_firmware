#pragma once

#include <stdint.h>
#include "ch.h"

extern mutex_t led_mutex;
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


void snowfox_led_init(void);
void snowfox_led_update_matrix(void);
void snowfox_led_on(void);
void snowfox_led_off(void);
static void snowfox_led_set_color(uint8_t key, uint8_t r, uint8_t g, uint8_t b) {
    led_matrix[led_map[key]] = r;
    led_matrix[led_map[key]+12] = g;
    led_matrix[led_map[key]+24] = b;
}

RgbColor HsvToRgb(HsvColor hsv);
