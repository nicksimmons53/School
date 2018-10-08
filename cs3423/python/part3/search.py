import re

string1 = "Hello world!"
string2 = "Hi Earth!"

# search for the fixed pattern "Hello" in strings
if re.search("Hello", string1):
    print("'" + string1 + "' contains 'Hello'")
else:
    print("'" + string1 + "' does not contain 'Hello'")

if re.search("Hello", string2):
    print("'" + string2 + "' contains 'Hello'")
else:
    print("'" + string2 + "' does not contain 'Hello'")

