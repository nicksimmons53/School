#!/usr/bin/env python

import subprocess

output = subprocess.check_output('ls -a', shell=True)

# get output as a string
print("---- STRING VERSION ----")
print(output)

# convert it to a list
print("\n---- LIST VERSION ----")
print(output.splitlines())
