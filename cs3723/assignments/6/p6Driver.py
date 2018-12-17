import sys
import re
from p5Dict import *
from itertools import islice

file = open(sys.argv[1], 'r')
lines = file.readlines( )
varTypeD = { }
varValueD = { }
labelD = { }

def eIf(line):
    token = line[1]
    var1 = line[2]
    var2 = line[3]
    label = line[4]

    if re.match('[a-zA-Z]+', var2):
        var2 = varValueD[var2.upper( )]
    if re.match('[a-zA-Z]+', var1):
        var1 = varValueD[var1.upper( )]

    if token == '>':
        if int(var1) > int(var2):
            return True
        else:
            return False

    if token == '>=':
        if int(var1) >= int(var2):
            return True
        else:
            return False

def eAssign(line):
    var1 = line[1]

    if len(line) == 5:
        token = line[2]
        var2 = line[3]
        value = line[4]
        if re.match('[a-zA-Z]+', var2):
            var2 = varValueD[var2.upper( )]
        if re.match('[a-zA-Z]+', value):
            value = varValueD[value.upper( )]

        if token == '+':
            varValueD[var1.upper( )] = int(var2) + int(value)
        if token == '-':
            varValueD[var1.upper( )] = int(var2) - int(value)
        if token == '*':
            varValueD[var1.upper( )] = str(var2 * int(value))
        if token == '&':
            varValueD[var1.upper( )] = str(var2 + value)

    if len(line) == 3:
        var2 = line[2]
        varValueD[var1.upper( )] = varValueD[var2.upper( )]

def ePrint(line):
    index = line.index('PRINT') + 1
    line = line[index:]
    string = ""
    for token in line:
        if re.match('".*"', token):
            string = string + token.strip('"') + " "
        else:
            string = string + str(varValueD[token.upper( )]) + " "
    print(string)

def eLoop(beforeLabel, loop):
    amount = int(re.findall('[0-9].*', beforeLabel)[0].strip(':'))
    b = eIf(loop[0][1:])
    label = loop[0][-1]
    count = 0

    while count < amount:
        for line in loop:
            if 'VAR' in line[0]:
                declareVar(line, varTypeD, varValueD)
            if 'PRINT' in line:
                ePrint(line)
            if 'ASSIGN' in line:
                eAssign(line)

        count += 1

def execute(verbose=False):
    count = 1
    firstCount = 1
    flag = 0
    label = ""
    beforeLabel = ""
    loopLabel = ""
    first = [ ]
    loop = [ ]

    #
    for line in lines:
        if line == '\n':
            continue

        line = line.strip('\n').strip( ).split( )[0]
        if re.match('.*:', line[0]):
            declareLabel(line, labelD, firstCount)
        if '#' in line:
            continue
        first.append(line)
        firstCount += 1

    #
    for line in lines:
        if line == '\n':
            continue

        line = line.strip('\n').split( )

        # GOTO
        if label != "":
            if label not in line:
                continue
            elif label in line:
                line = line[1:]
                label = ""

        # LOOP
        if loopLabel != "":
            if loopLabel not in line:
                loop.append(line)
                continue
            elif loopLabel in line:
                eLoop(beforeLabel, loop)
                loopLabel = ""
                loop = [ ]

        if '#' in line[0]:
            continue
        if re.match('[^Loop].*:', line[0]):
            if flag == 0:
                continue
            else:
                line = line[1:]

        if verbose != False:
            print("executing line %d: %s" %(count, ' '.join(line)))

        if re.match('^Loop.*', line[0]):
            b = eIf(line[1:])
            loop.append(line)
            beforeLabel = line[0].strip( )
            loopLabel = line[-1] + ":"
        if 'VAR' in line[0]:
            declareVar(line, varTypeD, varValueD)
        if 'PRINT' in line:
            ePrint(line)
        if 'ASSIGN' in line:
            eAssign(line)
        if 'IF' in line[0]:
            b = eIf(line)
            if b == False:
                continue
            else:
                label = line[4] + ":"
                continue
        if 'GOTO' in line[0]:
            label = line[1] + ":"

        count += 1

print("BEEP source code in %s:" %(sys.argv[1]))
count = 1
for line in lines:
    if line == '\n':
        continue

    line = line.split( )

    if '#' in line:
        continue
    if 'VAR' in line:
        declareVar(line, varTypeD, varValueD)
    if ':' in line[0]:
        declareLabel(line, labelD, count)
    print("%2d. %s" %(count, ' '.join(line)))
    count += 1

printVariables(varTypeD, varValueD)
printLabels(labelD, count)
print("execution begins ...")
if len(sys.argv) == 3:
    verbose = sys.argv[2]
    execute(verbose=True)
else:
    execute( )
