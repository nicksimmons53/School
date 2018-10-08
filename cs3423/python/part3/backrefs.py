#!/usr/bin/env python3

import re

def test_regex(regex, string):
    if re.search(regex, string):
        print("MATCH:\t\t" + regex + " \t'" + string + "'")
    else:
        print ("NO MATCH:\t" + regex + " \t'" + string + "'")

input1 = "sum = sum + 1"
input2 = "sum = sumthing + 1"
input3 = "sumthing = sum + 1"

regex1 = r"\s*(\w+)\s*=\s*\1.*"
regex2 = r"\s*(\w+)\s*=\s*\1\W*"
regex3 = r"\s*(\w+)\s*=\s*\1\W"

test_regex(regex1, input1)
test_regex(regex1, input2)
test_regex(regex1, input3)
print("---")
test_regex(regex2, input1)
test_regex(regex2, input2)
test_regex(regex2, input3)
print("---")
test_regex(regex3, input1)
test_regex(regex3, input2)
test_regex(regex3, input3)
