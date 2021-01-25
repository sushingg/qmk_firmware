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

#pragma once

#include "snowfox.h"
#include "quantum.h"
#include "ch.h"

extern uint8_t ble_led_status;
extern mutex_t ble_ok_mutex;
extern condition_variable_t ble_ok_cond;
extern uint8_t ble_ok;

extern THD_WORKING_AREA(waBLEThread, 128);
THD_FUNCTION(BLEThread, arg);

void snowfox_ble_startup(void);
void snowfox_ble_select(uint8_t port);
void snowfox_ble_discover(void);
void snowfox_ble_connect(void);
void snowfox_ble_disconnect(void);
bool snowfox_ble_is_active(void);
