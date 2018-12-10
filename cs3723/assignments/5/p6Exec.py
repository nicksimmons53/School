import re


def eAssign(line, varValueD):
    line = line.split( )
    if len(line) == 3:
        var = line[1].upper( ).strip( )
        var2 = line[2].upper( ).strip( )
        varValueD[var] = varValueD[var2]
    else:
        var = line[1].upper( ).strip( )
        varLiteral = line[3].upper( ).strip( )
        symbol = line[2].strip( )
        var2 = line[4].strip( )

        if re.match('[0-9]+', var2):
            var2 = int(var2)
        else:
            var2 = varValueD.get(var2.upper( ))

        if symbol == '+':
            varValueD[var] = int(varValueD[var]) + var2
        if symbol == '*':
            varValueD[var] = varValueD[varLiteral] * int(var2)
        if symbol == '-':
            varValueD[var] = int(varValueD[var]) - var2

def eGoto(label, lines):
    for line in lines:
        if label in line:
            return label

def eIf(line, varValueD, lines):
    line = line.split( )
    symbol = line[1]
    var1 = line[2].upper( )
    var2 = line[3].upper( )

    if symbol == '>':
        if varValueD[var1] > varValueD[var2]:
            return True
        else:
            return False

    if symbol == '>=':
        if varValueD[var1] > varValueD[var2]:
            return True
        else:
            return False

def ePrint(line, varValueD, count):
    line = line.split( )
    string = ""
    if (re.match('.*:.*', line[0])):
        line = line[1:]
    if (re.match('PRINT', line[0])):
        line = line[1:]

    for token in line:
        if re.match('".*"', token):
            string = string + token.strip('"') + " "
        else:
            string = string + str(varValueD[token.upper( )]) + " "
    print(string)

def eLoop(line, varValueD):
    line = line.split( )
    symbol = line[2]
    var1 = line[3]
    var2 = line[4].upper( )
    label = line[5].upper( )
    if symbol == '>':
        print(varValueD[var2])
        if int(var1) > int(varValueD[var2]):
            print(line)
            return True
        else:
            return False
    if symbol == '>=':
        if int(var1) >= int(varValueD[var2]):
            return True
        else:
            return False

def execute(lines, varValueD, labelD, verbose=None):
    count = 1;
    label = ""

    for line in lines:
        if "#" in line:
            continue
        if label != "":
            if label not in line:
                continue
        if 'VAR' in line:
            continue
        if re.match('^Loop.*', line):
            if eLoop(line, varValueD) == True:
                label = ""
                label = line.split( )[5]
                print(line)
                continue
        if 'IF' in line[0:3]:
            if verbose != None:
                print("executing line %d: %s" %(count, str(line).strip('\n').lstrip( )))
            if eIf(line, varValueD, lines) == True:
                continue
            else:
                label = line.split( )[4]
        if 'GOTO' in line:
            if verbose != None:
                print("executing line %d: %s" %(count, str(line).strip('\n').lstrip( )))
            line = line.split( )
            label = eGoto(line[1], lines)
        if 'ASSIGN' in line:
            if verbose != None:
                print("executing line %d: %s" %(count, str(line).strip('\n').lstrip( )))
            eAssign(line, varValueD)
        if 'PRINT' in line:
            if verbose != None:
                print("executing line %d: %s" %(count, str(line).strip('\n').lstrip( )))
            ePrint(line, varValueD, count)

        count += 1
