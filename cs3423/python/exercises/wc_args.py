#!/usr/bin/env python3

import sys

if len(sys.argv) < 2:
    print("Usage: " + sys.argv[0] + "<list_of_files>")
    sys.exit(1)

words = dict()

for path in sys.argv[1:]:
    try:
        with open(path, 'r') as f:
            for line in f:
                for word in line.split():
                    word = word.lower().strip(".!?,;\"'")
                    if word not in words:
                        words[word] = 1
                    else:
                        words[word] += 1
    except IOError:  # catches FileNotFound and PermissionError
        print("Error opening one of the files.")
        sys.exit(1)

sorted_words = list(words.keys())
sorted_words.sort()

for word in sorted_words:
    print(word + ": " + str(words[word]))
