#!/bin/bash

if [[ $# -lt 3 ]]; then
	echo "Usage: $0 requires three parameters"
	exit 1
fi

line_count=1

while read line; do
	if [[ $line_count -ge $2 ]] && [[ $line_count -le $3 ]]; then
		echo $line
	fi
	
	let line_count+=1
done < $1
