#!/usr/bin/env python3

import os
import sys

if len(sys.argv) != 2:
    print("Usage: " + sys.argv[0] + " <path>")
    sys.exit(1)

if os.path.isdir(sys.argv[1]):
    print("This is a directory!")
elif os.path.isfile(sys.argv[1]):
    print("This is a file!")
else:
    print("This is not a directory or a file!")

if os.access(sys.argv[1], os.W_OK):
    print("It’s also readable!")
else:
    print("You cannot read it!")
