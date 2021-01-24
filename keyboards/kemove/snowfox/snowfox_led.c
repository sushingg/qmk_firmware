#include "snowfox.h"

const SPIConfig spi1Config = {
  .clock_divider = 1, // No Division
  .clock_prescaler = 24, // To 2MHz
  .clock_rate = 4, // Divide 2 again to be 1MHz
  .data_size = 8, // 8 bits per transfer
};


THD_WORKING_AREA(waLEDThread, 128);
THD_FUNCTION(LEDThread, arg) {
    (void)arg;
    chRegSetThreadName("LEDThread");
    snowfox_led_init();
    snowfox_led_off();
    HsvColor currentColor = {.h = 0, .s = 0xFF, .v = 0xFF};
    while(1) {
        chThdSleepMilliseconds(50);
        currentColor.h += 1;
        RgbColor rgb = HsvToRgb(currentColor);
        for (int i = 0; i < 61; ++i)
        {
            snowfox_led_set_color(i, rgb.r, rgb.g, rgb.b);
        }
        snowfox_led_update_matrix();
    }
}