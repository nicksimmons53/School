#!/bin/bash

file_path=$1

sed -E -f command1.sed $file_path > ./first.c
sed -f command2.sed ./first.c > ./second.c
sed -E -f command3.sed ./second.c 
rm ./first.c ./second.c
