import re

string = "My student ID is aaa999. Yours is bbb222"

# redact all abc123 IDs
substituted = re.sub(r"[a-zA-Z]{3}\d{3}", "[REDACTED]", string)

# the original string remains unchanged
print("string is '" + string + "'")
print("substituted is '" + substituted + "'")
