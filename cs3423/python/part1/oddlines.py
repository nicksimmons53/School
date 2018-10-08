import sys

if len(sys.argv) != 3:
    print("Usage: " + sys.argv[0] + " <inpath> <outpath>")
    sys.exit(1)

line_number = 1

try:
    infile = open(sys.argv[1], 'r')

    # write odd lines to out file
    with open(sys.argv[2], 'w') as outfile:
        for line in infile:
            if line_number % 2:
                outfile.write(line)
            line_number += 1

except IOError:  # catches FileNotFound and PermissionError
    print("Error opening one of the files.")
    sys.exit(1)

# close in file (out file was closed with 'with' statement)
infile.close()
