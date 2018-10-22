import sys
import os

fileList = input("Enter files: ").split( )
for file in fileList:
    with open(file, 'r') as fp:
        lines = fp.readlines( )
        print(lines[0])
        print(lines[-1])
