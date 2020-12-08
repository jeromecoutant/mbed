#!/usr/bin/python

import sys
import os
import re

print ("File %s" %sys.argv[1])

support_arduino_uno = True

if len(sys.argv) > 2:
    if sys.argv[2] == "nano":
        print("ARDUINO NANO")
        support_arduino_uno = False

NEW_DEFINE = []

with open(sys.argv[1], "r") as sources:
    lines = sources.readlines()

LINE_TO_REMOVE = [
    " I2C_SDA ",
    " I2C_SCL ",
    " SPI_MOSI ",
    " SPI_MISO ",
    " SPI_SCK ",
    " SPI_CS ",
    " PWM_OUT ",
    "naming",
    "button names",
    "signals aliases",
    ]

with open("%s"%sys.argv[1], "w") as sources:
    user_button = "" # old button definition
    previous_pin = ""
    arduino_pins = 0
    for line in lines:
        pattern_LED = re.findall(" LED([\d])\s*=\s*([\w]+)", line)
        pattern_USER_BUTTON = re.findall("USER_BUTTON\s*=\s*([\w]+)", line)
        pattern_BUTTON = re.findall("BUTTON([\d])\s*=\s*([\w]+)", line)
        pattern_comment = re.findall("//([\s\w]+)", line)
        pattern_ADUINO = re.findall("(\s+)([AD][\d]+)\s*=\s*([\w]+)", line)

        if pattern_LED:
            if "LED\d" not in pattern_LED[0][1] and pattern_LED[0][1]!= previous_pin:
                new_define = "#define LED%-4s  %-5s" %(pattern_LED[0][0], pattern_LED[0][1])
                previous_pin = pattern_LED[0][1]
                if pattern_comment:
                    new_define += "  // %s" % pattern_comment[0].strip()
                NEW_DEFINE.append(new_define)
            continue

        elif pattern_USER_BUTTON:
            # USER_BUTTON definition is removed
            user_button = pattern_USER_BUTTON[0]
            continue

        elif pattern_BUTTON:
            if pattern_BUTTON[0][1] == "USER_BUTTON":
                button_pin = user_button
            else:
                button_pin = pattern_BUTTON[0][1]

            if button_pin != previous_pin:
                new_define = "#define BUTTON%s  %5s" % (pattern_BUTTON[0][0], button_pin)
                previous_pin = button_pin
                if pattern_comment:
                    new_define += "  // %s" % pattern_comment[0].strip()
                NEW_DEFINE.append(new_define)
            continue

        elif "STDIO_UART_TX =" in line:
            sources.write(line.replace("STDIO_UART_TX =", "USBTX ="))
            continue

        elif "STDIO_UART_RX =" in line:
            sources.write(line.replace("STDIO_UART_RX =", "USBRX ="))
            continue

        elif "= STDIO_UART" in line:
            # line removed
            continue

        elif support_arduino_uno and pattern_ADUINO:
            if arduino_pins == 0:
                sources.write("#ifdef TARGET_FF_ARDUINO_UNO\n")
            new_line = "%sARDUINO_UNO_%-3s = %s," %(pattern_ADUINO[0][0], pattern_ADUINO[0][1], pattern_ADUINO[0][2])
            if pattern_comment:
                new_line += "  // %s" % pattern_comment[0].strip()
            sources.write("%s\n" % new_line)
            arduino_pins += 1
            if arduino_pins == 22:
                sources.write("#endif\n")
            continue

        # elif support_arduino_uno and ("I2C" in line or " SPI" in line or "PWM_OUT" in line):
        #     # line removed
        #     print("removed: %s" %line.strip())
        #     pass

        elif "} PinName" in line:
            sources.write(line)

            if NEW_DEFINE:
                sources.write("\n// Standardized LED and button names\n")
                for led_define in NEW_DEFINE:
                    print(led_define)
                    sources.write("%s\n" %led_define.strip())
                sources.write("\n")
            continue

        line_approved = True
        for EachLineToRemove in LINE_TO_REMOVE:
            if EachLineToRemove in line:
                line_approved = False
        if line_approved:
            sources.write(line)

os.system("start " + sys.argv[1])
