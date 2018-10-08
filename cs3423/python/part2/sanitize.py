import sys

if len(sys.argv) != 2:
    print("Usage: " + sys.argv[0] + " <string>")
    sys.exit(1)

words = sys.argv[1].split()
sanitized = map(lambda w: w.strip(".,;!?'\"").lower(), words)
print(" ".join(sanitized))
