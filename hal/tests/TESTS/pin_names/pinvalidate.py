import re
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("path_to_PinNames")
args = parser.parse_args()

# todo: validate path

with open(args.path_to_PinNames) as pinNameFile:
    # extract body of PinName enum
    pinNameEnumMatch = re.search("typedef enum {\n([^}]*)\n} PinName;", pinNameFile.read())
    # todo: verify PinName enum was found
    pinNameEnumBody = pinNameEnumMatch.group(1)

    # extract enum values into dictionary
    pinNameVals = dict(re.findall("^\s*([a-zA-Z0-9_]+)\s*=\s*([a-zA-Z0-9_]+)", pinNameEnumBody, re.MULTILINE))
    
    used_pins = []
    for key, val in pinNameVals.items():
        if val in used_pins:
            print(key + ' = ' + val + '\tInvalid assignment: duplicate value')
            continue

        if val == 'NC':
            print(key + ' = ' + val + '\tInvalid assignment: cannot be NC')
            continue

        if re.match("^(LED|BUTTON)\d*", val):
            print(key + ' = ' + val + '\tInvalid assignment: cannot be another LED or BUTTON')
            continue

        if not re.match("^P[A-J]_\d+", val):
            used_pins.append(val)