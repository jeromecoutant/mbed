# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(TARGET_NUCLEO_L552ZE_Q EXCLUDE_FROM_ALL)
add_subdirectory(TARGET_NUCLEO_L552ZE_Q_NPSA EXCLUDE_FROM_ALL)

add_library(mbed-stm32l552xe INTERFACE)

target_sources(mbed-stm32l552xe
    INTERFACE
        ${STARTUP_FILE}
)

target_include_directories(mbed-stm32l552xe
    INTERFACE
        .
)

mbed_set_linker_script(mbed-stm32l552xe ${CMAKE_CURRENT_SOURCE_DIR}/${LINKER_FILE})

target_link_libraries(mbed-stm32l552xe INTERFACE mbed-stm32l5)
