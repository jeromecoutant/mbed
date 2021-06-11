# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

if(${MBED_TOOLCHAIN} STREQUAL "GCC_ARM")
    set(STARTUP_FILE TOOLCHAIN_GCC_ARM/startup_stm32l552xx.S)
    set(LINKER_FILE TOOLCHAIN_GCC_ARM/stm32l552xe.ld)
elseif(${MBED_TOOLCHAIN} STREQUAL "ARM")
    set(STARTUP_FILE TOOLCHAIN_ARM/startup_stm32l552xx.S)
    set(LINKER_FILE TOOLCHAIN_ARM/stm32l552xe.sct)
endif()

add_library(mbed-nucleo-l552ze-q INTERFACE)

target_sources(mbed-nucleo-l552ze-q
    INTERFACE
        PeripheralPins.c
)

target_include_directories(mbed-nucleo-l552ze-q
    INTERFACE
        .
)

target_link_libraries(mbed-nucleo-l552ze-q INTERFACE mbed-stm32l552xe)
