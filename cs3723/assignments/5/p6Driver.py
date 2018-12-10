import sys
import os
import re
from p5Dict import *
from p6Exec import *

fileName = sys.argv[1]
if (len(sys.argv) == 4):
    switch = sys.argv[3]
inputFile = open(fileName, 'r')
lines = inputFile.readlines( )[2:];
varValueD = { }
varTypeD = { }
labelD = { }

def parseLine(lines):
    count = 1

    print("BEEP source code in %s" %(fileName))
    for line in lines:
        if re.search('^#', line):
            continue

        print("%3d. %s" %(count, line.strip('\n')))

        if re.search('[a-z0-9A-Z]+', line):
            parsedLine = line.split( )
            if parsedLine[0] == 'VAR':
                declareVar(parsedLine, varTypeD, varValueD)
            if ":" in parsedLine[0]:
                declareLabel(parsedLine, labelD, count)

        count += 1

    printVariables(varTypeD, varValueD)
    printLabels(labelD, count)

parseLine(lines)

print("execution begins ...")
if len(sys.argv) == 3:
    execute(lines, varValueD, labelD, verbose=sys.argv[2])
else:
    execute(lines, varValueD, labelD)
