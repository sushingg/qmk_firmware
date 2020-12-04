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

#include <string.h>
#include "ch.h"
#include "hal.h"
#include "wait.h"
#include "snowfox.h"
#include "board.h"
#include "sled17341.h"

/* ============ Private Defines ===================== */

/* ============ Function Prototypes ================== */

const ioline_t row_list[MATRIX_ROWS] = {
    LINE_ROW1,
    LINE_ROW2,
    LINE_ROW3,
    LINE_ROW4,
    LINE_ROW5,
    LINE_ROW6,
    LINE_ROW7,
    LINE_ROW8,
};

const ioline_t col_list[MATRIX_COLS] = {
    LINE_COL1,
    LINE_COL2,
    LINE_COL3,
    LINE_COL4,
    LINE_COL5,
    LINE_COL6,
    LINE_COL7,
    LINE_COL8,
    LINE_COL9,
};

void __chibios_override___early_init(void) {
}

const SPIConfig spi1Config = {
  .clock_divider = 1, // No Division
  .clock_prescaler = 24, // To 2MHz
  .clock_rate = 1, // Divide 2 again to be 1MHz
  .data_size = 8, // 8 bits per transfer
};

/**
 * @brief   Board-specific initialization code.
 * @todo    Add your board-specific code, if any.
 */
void __chibios_override_boardInit(void) {
    // ISP TP
    palSetLineMode(LINE_LED, MODE_DIR_IN | MODE_MODE_PULL_UP | MODE_AD_DIGITAL);
    // USB
    palSetLineMode(LINE_USBVBUS, MODE_FUNC_ALT1 | MODE_MODE_PULL_UP | MODE_AD_DIGITAL);
    palSetLineMode(LINE_USBCONN, MODE_FUNC_ALT1);
    // LEDs
    palSetLineMode(LINE_LED1_CS, MODE_DIR_OUT | MODE_AD_DIGITAL);
    palSetLineMode(LINE_LED2_CS, MODE_DIR_OUT | MODE_AD_DIGITAL);
    palSetLineMode(LINE_SSP1_MOSI, MODE_FUNC_ALT2 | MODE_AD_DIGITAL); // SSP1 MOSI
    palSetLineMode(LINE_SSP1_MISO, MODE_FUNC_ALT2 | MODE_AD_DIGITAL); // SSP1 MISO
    palSetLineMode(LINE_SSP1_SCK, MODE_FUNC_ALT3 | MODE_AD_DIGITAL); // SSP1 SCK
    palSetLine(LINE_LED1_CS);
    palSetLine(LINE_LED2_CS);

    spiStart(&SPID1, &spi1Config);

    palSetLineMode(LINE_BLE_RSTN, MODE_DIR_OUT | MODE_AD_DIGITAL);
    palSetLineMode(LINE_UART_RX, MODE_FUNC_ALT2 | MODE_AD_DIGITAL);
    palSetLineMode(LINE_UART_TX, MODE_FUNC_ALT2 | MODE_AD_DIGITAL);
}
