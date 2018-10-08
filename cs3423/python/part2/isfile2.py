import sys
import os.path

filtered = filter(os.path.isfile, sys.argv[1:])
print(list(filtered))
