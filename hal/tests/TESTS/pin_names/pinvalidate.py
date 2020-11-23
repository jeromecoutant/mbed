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
        # generic checks
        if val == key:
            print(key + ' = ' + val + '\tInvalid assignment: cannot assign value to itself')
            continue
        if val == 'NC':
            print(key + ' = ' + val + '\tInvalid assignment: cannot be NC')
            continue

        # LED/BUTTON/UART checks
        if re.match("^((LED|BUTTON)\d*|USBTX|USBRX)$", key):
            # resolve to literal
            realval = val
            while not re.match("(0x[0-9a-fA-F]+|[1-9][0-9]*|0[1-7][0-7]+|0b[01]+)[uUlL]{0,2}", realval):
                try:
                    realval = pinNameVals[realval]
                except:
                    print(key + " does not resolve to a literal!")
                    break

            if realval in used_pins:
                print(key + ' = ' + val + '\tInvalid assignment: ' + realval + ' already assigned')
                continue

            used_pins.append(realval)