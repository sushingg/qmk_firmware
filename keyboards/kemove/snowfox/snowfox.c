#include "snowfox.h"

THD_WORKING_AREA(waLEDThread, 128);
THD_FUNCTION(LEDThread, arg) {
    (void)arg;
    chRegSetThreadName("LEDThread");
    led_init();
    led_on();
    HsvColor currentColor = {.h = 0, .s = 0xFF, .v = 0xFF};
    while(1) {
        chThdSleepMilliseconds(50);
        currentColor.h += 1;
        RgbColor rgb = HsvToRgb(currentColor);
        for (int i = 0; i < 61; ++i)
        {
            led_set_color(i, rgb.r, rgb.g, rgb.b);
        }
        led_update_matrix();
    }
}

void matrix_scan_kb(void) {
    matrix_scan_user();
}

void matrix_init_kb(void) {
    (void) chThdCreateStatic(waLEDThread, sizeof(waLEDThread), NORMALPRIO, LEDThread, NULL);
    matrix_init_user();
}
