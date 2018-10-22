import sys
import os
import subprocess
output = subprocess.check_output('ps -ef | grep -F -w "root"', shell=True, stderr=subprocess.STDOUT)
for line in output.splitlines( )[::2]:
    print(line.decode('utf-8')))
