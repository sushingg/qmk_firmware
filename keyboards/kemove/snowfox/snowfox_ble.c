/*
    Copyright (C) 2020 Yaotian Feng, Codetector<codetector@codetector.cn>

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "snowfox_ble.h"
#include "ch.h"
#include "hal.h"
#include "host.h"
#include "host_driver.h"
#include <string.h>

uint8_t ble_led_status = 0;
mutex_t ble_ok_mutex;
condition_variable_t ble_ok_cond;
uint8_t ble_ok;

/* -------------------- Static Function Prototypes -------------------------- */
static uint8_t snowfox_ble_leds(void);
static void snowfox_ble_mouse(report_mouse_t *report);
static void snowfox_ble_keyboard(report_keyboard_t *report);
static void snowfox_ble_system(uint16_t data);
static void snowfox_ble_consumer(uint16_t data);

static void snowfox_ble_swtich_ble_driver(void);

/* -------------------- Static Local Variables ------------------------------ */
static host_driver_t snowfox_ble_driver = {
    snowfox_ble_leds,
    snowfox_ble_keyboard,
    snowfox_ble_mouse,
    snowfox_ble_system,
    snowfox_ble_consumer,
};

static host_driver_t *lastHostDriver = NULL;
#ifdef NKRO_ENABLE
static bool lastNkroStatus = false;
#endif // NKRO_ENABLE


/* -------------------- Static Function Implementation ---------------------- */
static uint8_t snowfox_ble_leds(void) {
    return ble_led_status;
}
static void snowfox_ble_mouse(report_mouse_t *report) {}
static void snowfox_ble_keyboard(report_keyboard_t *report) {
    uint8_t buffer[15] = {'A', 'T', '+', 'H', 'I', 'D','=', 0x1,
    report->raw[0], report->raw[2], report->raw[3], report->raw[4], report->raw[5], report->raw[6], report->raw[7]};
    sdWrite(&SD1, buffer, 15);
}
static void snowfox_ble_system(uint16_t data) {}
static void snowfox_ble_consumer(uint16_t data) {}

static void snowfox_ble_swtich_ble_driver(void) {
    if (host_get_driver() == &snowfox_ble_driver) {
        return;
    }
    clear_keyboard();
    lastHostDriver = host_get_driver();
#ifdef NKRO_ENABLE
    lastNkroStatus = keymap_config.nkro;
#endif
    keymap_config.nkro = false;
    host_set_driver(&snowfox_ble_driver);
}

static void snowfox_restore_driver(void) {
    /* Skip if the driver is already enabled */
    if (host_get_driver() != &snowfox_ble_driver) {
        return;
    }

    clear_keyboard();
#ifdef NKRO_ENABLE
    keymap_config.nkro = lastNkroStatus;
#endif
    host_set_driver(lastHostDriver);
}

static void ble_command(const char* cmd) {
    sdWrite(&SD1, (uint8_t*)cmd, strlen(cmd));
}

static uint8_t ble_check_ok(void) {
    msg_t t;
    do {
        t = sdGetTimeout(&SD1, TIME_MS2I(10));
        if (t < MSG_OK) {
            return 0;
        }
    } while (t != 'O');
    t = sdGetTimeout(&SD1, TIME_MS2I(10));
    if (t == 'K') {
        t = sdGetTimeout(&SD1, TIME_MS2I(10));
        if (t == '\r') {
            t = sdGetTimeout(&SD1, TIME_MS2I(10));
            if (t == '\n') {
                return 1;
            }
        }
    }
    return 0;
}

#define BLE_RESP_BUF_CAP    32
static uint8_t ble_resp_buffer[BLE_RESP_BUF_CAP];
static uint8_t ble_resp_buffer_size = 0;

static void process_response(void) {
    if (strncmp("OK", (char*)ble_resp_buffer, 2) == 0) {
        chMtxLock(&ble_ok_mutex);
        ble_ok = 1;
        chCondSignal(&ble_ok_cond);
        chMtxUnlock(&ble_ok_mutex);
    } else if (strncmp("+LEDPAGE:", (char*)ble_resp_buffer, 9) == 0) {
        // LEDPAGE
        if (ble_resp_buffer[9] <= '9' && ble_resp_buffer[9] >= '0') {
            ble_led_status = ble_resp_buffer[9] - (uint8_t)'0';
        } else {
            ble_led_status = 0; // I don't expect this to happen but we shall see.
        }
    } else if (strncmp("+EVENT:0008", (char*)ble_resp_buffer, 11) == 0) {
        snowfox_restore_driver();
    }
}

/* BLE THREAD */
THD_WORKING_AREA(waBLEThread, 128);
THD_FUNCTION(BLEThread, arg) {
    (void)arg;
    chRegSetThreadName("BLEThread");
    snowfox_ble_startup();
    while(1) {
        msg_t msg = sdGetTimeout(&SD1, TIME_MS2I(10));
        if (msg >= MSG_OK) {
            if (msg == '\r' || msg == '\n') {
                if (ble_resp_buffer_size > 0) {
                    process_response();
                }
                ble_resp_buffer_size = 0;
            } else {
                ble_resp_buffer[ble_resp_buffer_size++] = (uint8_t)msg;
            }
        }
    }
}

/* -------------------- Public Function Implementation ---------------------- */

void snowfox_ble_startup(void) {
    sdStart(&SD1, &serialCfg);
    palClearLine(LINE_BLE_RSTN);
    chThdSleepMilliseconds(1);
    palSetLine(LINE_BLE_RSTN);
    chThdSleepMilliseconds(500);
    ble_command("\r\n");
    ble_command("AT+UART=115200\r\n");
    sdStop(&SD1);
    serialCfg.speed=115200;
    sdStart(&SD1, &serialCfg);
    chThdSleepMilliseconds(300);
    while(1) {
        ble_command("AT\r\n");
        chThdSleepMilliseconds(50);
        if (ble_check_ok()) {
            break;
        }
    }
    ble_command("AT+UART=115200\r\n");
    ble_check_ok();
    ble_command("AT+KEYBOARD=1\r\n");
    ble_check_ok();
    ble_command("AT+NAME=SnowfoxQMK\r\n");
    ble_check_ok();
    ble_command("AT+DISCONN\r\n");
    ble_check_ok();
}

static void ble_command_withOk(const char* string) {
    chMtxLock(&ble_ok_mutex);
    ble_ok = 0;
    ble_command(string);
    while(ble_ok == 0) {
        chCondWait(&ble_ok_cond);
    }
    ble_ok = 0;
    chMtxUnlock(&ble_ok_mutex);
}

void snowfox_ble_select(uint8_t port) {
    chMtxLock(&ble_ok_mutex);
    ble_ok = 0;
    switch (port) {
        case 1:
            ble_command("AT+KEYBOARD=1\r\n"); break;
        case 2:
            ble_command("AT+KEYBOARD=2\r\n"); break;
        case 3:
            ble_command("AT+KEYBOARD=3\r\n"); break;
        default:
            chMtxUnlock(&ble_ok_mutex); return;
    }
    while(ble_ok == 0) {
        chCondWait(&ble_ok_cond);
    }
    ble_ok = 0;
    chMtxUnlock(&ble_ok_mutex);
}
void snowfox_ble_discover(void) {
    ble_command_withOk("AT+DISCOVER\r\n");
}
void snowfox_ble_connect(void) {
    ble_command_withOk("AT+CONN\r\n");
    snowfox_ble_swtich_ble_driver();
}
void snowfox_ble_disconnect(void) {
    ble_command_withOk("AT+DISCONN\r\n");
    snowfox_restore_driver();
}
