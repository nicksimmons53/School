#!/bin/bash

if [ ! -d ./data ]; then
    mkdir ./data/
fi 

go=0
while [ $go ]; do
    echo "Enter one of the following actions or press CTRL-D to exit"
    echo "C - create a new item"
    echo "R - read an existing item"
    echo "U - update an existing item"
    echo "D - delete an existing item" 

    if ! read arg; then
        break
    fi

    case "$arg" in
        [cC]) ./create.bash;;
        [rR]) ./read.bash;;
        [uU]) ./update.bash;;
        [dD]) ./delete.bash;;
           *) echo "ERROR: invalid option"
              ./assign1.bash;;
    esac
done
