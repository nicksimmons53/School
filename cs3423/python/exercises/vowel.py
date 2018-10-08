#!/usr/bin/env python3

import sys

VOWELS = ['a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U']

if len(sys.argv) != 2:
    print("Usage: " + sys.argv[0] + " <string>")
    sys.exit(1)

for letter in sys.argv[1]:
    if letter in VOWELS:
        break
    print(letter)

print(len(sys.argv[1]))
