"""
Copyright (c) 2017 ARM Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import re
import argparse

def pinName_to_dict(pinName_file_content):
    pinName_enum_match = re.search("typedef enum {\n([^}]*)\n} PinName;", pinName_file_content)
    pinName_enum_body = pinName_enum_match.group(1)
    return dict(re.findall("^\s*([a-zA-Z0-9_]+)\s*=\s*([a-zA-Z0-9_]+)", pinName_enum_body, re.MULTILINE))

def identity_assignment_check(pinName_dict):
    for key, val in pinName_dict.items():
        if val == key:
            print(key + ' = ' + val + '\tInvalid assignment: cannot assign value to itself')

def nc_assignment_check(pinName_dict):
    for key, val in pinName_dict.items():
        if val == 'NC':
            print(key + ' = ' + val + '\tInvalid assignment: cannot be NC')

def duplicate_assignment_check(pinName_dict):
    used_pins = []
    used_pins_friendly = []

    for key, val in pinName_dict.items():
        if re.match("^((LED|BUTTON)\d*|USBTX|USBRX)$", key):
            # resolve to literal
            realval = val
            while not re.match("(0x[0-9a-fA-F]+|[1-9][0-9]*|0[1-7][0-7]+|0b[01]+)[uUlL]{0,2}", realval):
                try:
                    realval = pinName_dict[realval]
                except:
                    print(key + " does not resolve to a literal!")
                    break

            if realval in used_pins:
                print(key + ' = ' + val + '\tInvalid assignment: already assigned (' + used_pins_friendly[used_pins.index(realval)] + ')')
                continue

            used_pins.append(realval)
            used_pins_friendly.append(key + ' = ' + val)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("path_to_PinNames")
    args = parser.parse_args()

    try:
        pinNameFile = open(args.path_to_PinNames)
    except:
        exit("Cannot open file")
    
    try:
        pinName_dict = pinName_to_dict(pinNameFile.read())
    except:
        exit("Cannot extract PinName enum from file")
    
    identity_assignment_check(pinName_dict)
    nc_assignment_check(pinName_dict)
    duplicate_assignment_check(pinName_dict)

if __name__ == "__main__":
    main()