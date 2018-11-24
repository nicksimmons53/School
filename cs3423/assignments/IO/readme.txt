# empMerge can merge two binary files of employee records
# to create the executable
gcc -o empMerge empMerge.c
# To create a binary date file from 
# a text file, use empMerge -i txtFile -n masterFile
./empMerge -i input.txt -n master.dat
./empMerge -i update.txt -n update.dat
# To merge, use 
./empMerge -m master.dat -u update.dat -n newMaster.dat

