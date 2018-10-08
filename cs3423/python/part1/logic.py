#!/usr/bin/env python3

import sys

if len(sys.argv) != 2:
    print("Usage: " + sys.argv[0] + " <string>")
    sys.exit(1)

str_length = len(sys.argv[1])

if str_length > 5 and not str_length % 2:
    print("'" + sys.argv[1] + "' is longer than 5 chars and has even length.")
