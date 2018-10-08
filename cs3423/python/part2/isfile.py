import sys
import os.path

filtered = list()
for arg in sys.argv[1:]:
    if os.path.isfile(arg):
        filtered.append(arg)

print(filtered)
