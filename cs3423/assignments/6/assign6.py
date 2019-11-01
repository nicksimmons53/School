#!/usr/bin/env python3
import os
import sys
import glob
import re

def fileMove(directory, file):
    if (os.path.isdir(directory) == False):
        os.makedirs(directory)
    os.system("mv " + file + " " + directory)

def assignment6(files, dir, zipFile=False):
    pattern = "^proj([0-9a-zA-Z]+)"
    projPattern = "^proj"

    for file in files:
        newFile = os.path.split(file)[-1]
        matchFile = re.match(pattern, newFile)
        matchProj = re.match(projPattern, newFile)

        if (matchFile):
            newDir = dir + "/assignment" + matchFile.group(1);
            fileMove(newDir, file)
        elif (matchProj):
            newDir = dir + "/assignment"
            fileMove(newDir, file)
        elif (os.path.isfile(file)):
            newDir = dir + "/misc"
            fileMove(newDir, file)

    if (zipFile):
        if (re.match(zipFile, "misc")):
            zipDirectory = zipFile
        elif (re.match("assignment$", zipFile)):
            zipDirectory = zipFile
        else:
            zipDirectory = "assignment" + zipFile + "$"
        for directory in os.listdir(dir):
            if re.search(zipDirectory + "$", directory):
                zipFileName = directory + ".zip"
                os.system("zip " + zipFileName + " " + dir + "/" + directory + "/*")
                os.system("mv " + zipFileName + " " + dir)


if not os.path.isdir(sys.argv[1]):
    print(sys.argv[1] + " is not a dir.")
    sys.exit(1)

dir = sys.argv[1]
files = glob.glob(dir + "/*")

if len(sys.argv) == 2:
    assignment6(files, dir)
elif len(sys.argv) == 3:
    zipFile = sys.argv[2]
    assignment6(files, dir, zipFile)
else:
    print("Usage: " + sys.argv[0] + "<directory>")
    sys.exit(1)
