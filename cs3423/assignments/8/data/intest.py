import sys
# use this with intest.txt to test input redirection

for line in sys.stdin:
    print("read: " + str(line).strip())

print("done reading!")
