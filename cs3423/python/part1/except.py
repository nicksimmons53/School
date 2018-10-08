#!/usr/bin/env python3

valid = False
while not valid:
    try:
        a = int(input("Enter an integer: "))
        b = int(input("Enter another integer: "))
        valid = True  # the above lines didn't cause an exception
    except ValueError as e:
        print("You didn't enter an int! Start over.")
        valid = False
    except Exception as e:
        print("An exception occurred that I didn't account for: " + str(e))
        valid = False

total = a + b
print("Sum: " + str(total))
