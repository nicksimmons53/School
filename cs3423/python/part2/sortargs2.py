import sys

# extract and sort args
words = sys.argv[1:]
words.sort()

print(" ".join(words))
