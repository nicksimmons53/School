#!/usr/bin/env python3


# print a string one letter at a time, optionally backwards
def letter_lines(string, backwards=False):
    if backwards:
        string = string[::-1]

    for letter in string:
        print(letter)


# parse yes or other to Boolean
def parse_yn(decision):
    if decision in ['y', 'Y', 'YES', 'yes']:
        return True
    return False

instring = input("Enter a string: ")
back = parse_yn(input("Backwards? (y/n): "))

# call with and without optional parameter
letter_lines(instring, backwards=back)
letter_lines(" DONE")
