#!/usr/bin/env python3

import os
import sys
import glob

count = 0

# check for number of arguments
if len(sys.argv) != 3:
    print("Usage: " + sys.argv[0] + " <dirname> <size>")
    sys.exit(1)

# check that the first arg is a directory
if not os.path.isdir(sys.argv[1]):
    print(sys.argv[1] + " is not a directory")
    sys.exit(1)

# get the .py files
py_files = glob.glob(sys.argv[1] + "/*.py")

# check their sizes
for pf in py_files:
    stat = os.stat(pf)
    if stat.st_size > int(sys.argv[2]):
        count += 1
        print(pf + " - " + str(stat.st_size))

print(str(count) + " .py files found with size greater than "
      + sys.argv[2])
