#!/usr/bin/env python3

import sys

if len(sys.argv) != 2:
    print("Usage: " + sys.argv[0] + " <string>")
    sys.exit(1)

for letter in sys.argv[1]:
    print(letter)
