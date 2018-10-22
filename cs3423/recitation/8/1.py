import os
import sys
for file in sys.argv[1:]:
	with open(file, "r") as fp:
		lines = fp.readlines( )
		print (lines[0])
		print (lines[-1])

