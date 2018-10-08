import sys

if len(sys.argv) != 3:
    print("Usage: " + sys.argv[0] + " <inpath> <outpath>")
    sys.exit(1)

try:
    # extract odd lines
    with open(sys.argv[1], 'r') as infile:
        oddlines = infile.readlines()[::2]

    # write odd lines to out file
    with open(sys.argv[2], 'w') as outfile:
        outfile.write("".join(oddlines))

except IOError:  # catches FileNotFound and PermissionError
    print("Error opening one of the files.")
    sys.exit(1)
