#!/usr/bin/env python3

import sys

if len(sys.argv) != 2:
    print("Usage: " + sys.argv[0] + " <string>")
    sys.exit(1)

count = 0
word = sys.argv[1]

while count < len(word):
    print(word[count])
    count += 1
