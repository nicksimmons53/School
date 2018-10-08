import sys
import re

# skipping validation for brevity

pattern = r"^([^:]*):[^:]*:[^:]*:[^:]*:([^:]*):[^:]*:([^:]*)$"

try:
    with open(sys.argv[1], 'r') as passwd:
        for line in passwd:
            matches = re.search(pattern, line.rstrip())
            if matches:
                print(matches.group(2) + "'s username is " +
                      matches.group(1) + " and they use " +
                      matches.group(3) + " as their shell.")
except IOError:
    print("Error opening file")
    sys.exit(1)
