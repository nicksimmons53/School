import sys
import os
import re
from p5Dict import *

inputFile = open(sys.argv[1], 'r')
lines = inputFile.readlines( );

def parseLine(lines):
    varTypeD = { }
    varValueD = { }
    labelD = { }
    count = 1

    for line in lines:
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
