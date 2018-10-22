#!/bin/bash

for file in $@; do
	echo $(ls -l $file/*)
	echo ""
done
