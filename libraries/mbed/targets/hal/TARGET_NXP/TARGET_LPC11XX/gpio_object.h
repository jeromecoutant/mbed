/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_GPIO_OBJECT_H
#define MBED_GPIO_OBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    PinName  pin;
    __IO uint32_t *mask;
    __IO uint32_t *reg_dir;
    __IO uint32_t *reg_data;
    __I  uint32_t *reg_in;
} gpio_t;

static inline void gpio_write(gpio_t *obj, int value) {
    uint32_t pin_number = ((obj->pin & 0x0F00) >> 8);
    if (value)
        *obj->reg_data |= (1 << pin_number);
    else
        *obj->reg_data &= ~(1 << pin_number);
}

static inline int gpio_read(gpio_t *obj) {
    return ((*obj->mask) ? 1 : 0);
}

#ifdef __cplusplus
}
#endif

#endif
