import re
import sys

try:
    with open(sys.argv[1], 'r') as infile:
        # read the entire file into one string
        # rstrip() to remove the empty string read() adds at EOF
        all_text = infile.read().rstrip()
        print("File contents as a single variable:\n" + all_text)

        # add <h1> to the beginning of every line
        subbed = re.sub(r"^", "<h1>", all_text, flags=re.MULTILINE)

        # add </h1> to the end of every line
        subbed = re.sub(r"$", "</h1>", subbed, flags=re.M)

        # redact all abc123s ignoring case
        subbed = re.sub(r"[a-z]{3}\d{3}", "[REDACTED]", subbed, flags=re.M|re.I)

        print("\nSubstitued:\n" + subbed)
except IOError:
    print("Error opening file")
    sys.exit(1)
