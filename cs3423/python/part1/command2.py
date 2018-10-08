#!/usr/bin/env python3.5
# Note that subprocess.run() only works for 3.5 or higher

import subprocess

process = subprocess.run(['ls', '-a'], stdout=subprocess.PIPE, encoding='utf-8')
output = process.stdout

# get output as a string
output_str = process.stdout
print("---- STRING VERSION ----")
print(output_str)

# convert it to a list
output_list = output_str.splitlines()
print("\n---- LIST VERSION ----")
print(output_list)
