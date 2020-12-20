#include "snowfox.h"
#include "snowfox_ble.h"
#include "string.h"

thread_t *bleThread;

SerialConfig serialCfg = {
    9600
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

void matrix_scan_kb(void) {
    matrix_scan_user();
}


void matrix_init_kb(void) {
    chMtxObjectInit(&ble_ok_mutex);
    chCondObjectInit(&ble_ok_cond);
    #ifdef SNOWFOX_V3
    (void) chThdCreateStatic(waLEDThread, sizeof(waLEDThread), NORMALPRIO, LEDThread, NULL);
    #endif
    bleThread = chThdCreateStatic(waBLEThread, sizeof(waBLEThread), NORMALPRIO, BLEThread, NULL);
    matrix_init_user();
}

void bootloader_jump(void) {
    *((volatile uint32_t*) 0x100001F0) = 0xDEADBEEF;
    __asm__ __volatile__("dsb");
    SCB->AIRCR = 0x05FA0004; // Sys Reset
    __asm__ __volatile__("dsb");
    while(1) {}
}

/*!
 * @returns false   processing for this keycode has been completed.
 */
bool OVERRIDE process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            case SNOWFOX_LED_ON:
                snowfox_led_on();
                return false;
            case SNOWFOX_LED_OFF:
                snowfox_led_off();
                return false;
            case SNOWFOX_BLE_CONN:
                snowfox_ble_connect();
                return false;

            case SNOWFOX_BLE_DISCOVER:
                snowfox_ble_discover();
                return false;

            case SNOWFOX_BLE_DISCONN:
                snowfox_ble_disconnect();
                return false;

            default:
                break;
        }
    }
    return process_record_user(keycode, record);
}
