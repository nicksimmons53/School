import glob
import os

for file in glob.glob("*.c"):
    if os.access(file, os.R_OK):
        print(file)
