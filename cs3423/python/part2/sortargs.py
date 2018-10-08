import sys

# extract and sort args
words = sys.argv[1:]
words.sort()

for word in words:
    print(word)
