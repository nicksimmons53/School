import re

# check for valid synonym for aiplane
def is_airplane(string):
    if re.search(r"(air(craft|plane)|jet)", string):
        print("'" + string + "' has a plane in it!")
    else:
        print("'" + string + "' has no plane in it.")


is_airplane("The airplane flew high")
is_airplane("The aircraft flew high")
is_airplane("The jet flew high")
is_airplane("The air is high")
