def declareVar(line, varTypeD, varValueD):
    if (len(line)) == 4:
        type = line[1].upper( )
        name = line[2].upper( )
        value = line[3].strip('"')
    elif (len(line)) == 3:
        type = line[1].upper( )
        name = line[2].upper( )
        value = ""

    varTypeD[name] = type
    varValueD[name] = value

def declareLabel(line, labelD, count):
    label = line[0].upper( ).strip(':')
    if label in labelD.keys( ):
        print("\t***ERROR: label '%s' occurs appears on multiple lines: %d and %d" %(label.strip(':'), labelD[label], count))
    labelD[label] = count

def printVariables(varTypeD, varValueD):
    print("Variables: ")
    print("\t%-15s %-10s %-50s" %("Variable", "Type", "Value"))
    for val in sorted(varTypeD):
        value = varValueD[val]
        print("\t%-15s %-10s %-50s" %(val, varTypeD[val], varValueD[val]))

def printLabels(labelD, count):
    print("Labels: ")
    print("\t%-15s %s" %("Label", "Statement"))
    for val in sorted(labelD):
        print("\t%-15s %-d" %(val, labelD[val]))
