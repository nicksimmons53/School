#!/usr/bin/env python3

# continuously read from stdin
while True:
    # stop when EOF is reached (or ctrl+d)
    try:
        course = input("Enter your favorite course: ")
    except EOFError:
        print("done reading!")
        break

    if course == "systems" or course == "systems programming":
        print("good choice")
    elif course == "software engineering":
        print("not bad")
    else:
        print("wrong")

print("done with loop!")
