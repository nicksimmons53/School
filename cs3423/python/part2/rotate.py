import sys

if len(sys.argv) < 3:
    print("Usage: " + sys.argv[0] + " <multiple_args>")
    sys.exit(1)

# remove the program name from the arguments
sys.argv.pop(0)

# remove and store the first argument
temp = sys.argv.pop(0)

# add it back to the end
sys.argv.append(temp)
print(sys.argv)
