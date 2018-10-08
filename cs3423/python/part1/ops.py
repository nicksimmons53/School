#!/usr/bin/env python3

import sys

if len(sys.argv) != 3:
    print("Usage: " + sys.argv[0] + " <int> <int>")
    sys.exit(1)

# note that this can result in an error if ints aren't passed
# we'll learn how to handle this later
int_add = int(sys.argv[1]) + int(sys.argv[2])
str_add = sys.argv[1] + sys.argv[2]  # sys.argv contains strings

print("Adding as integers results in " + str(int_add))
print("Adding as strings results in " + str_add)
