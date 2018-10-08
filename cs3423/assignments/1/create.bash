#!/bin/bash

read -p "Enter Item Number: " item_number
read -p "Enter a Simple Name: " simple_name
read -p "Enter Item Name: " item_name
read -p "Enter Current Quantity: " current_quantity
read -p "Enter Maximum Quantity: " max_quantity
read -p "Enter Item Description: " description

file_name="$item_number.item"
file_path="./data/$file_name"

if [ -f $file_path ]; then
    echo "ERROR: item already exists"
    exit 1
fi

echo -e "$simple_name $item_name\n$current_quantity $max_quantity\n$description" > $file_path
echo -e "CREATED: $simple_name - `date`" >> ./data/queries.log
