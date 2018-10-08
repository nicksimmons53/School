#!/bin/bash

read -p "Enter an Item Number: " item_number
read -p "Enter a Simple Name: " new_simple_name
read -p "Enter an Item Name: " new_item_name
read -p "Enter a Current Quantity: " new_current_quantity
read -p "Enter a Maximum Quantity: " new_max_quantity
read -p "Enter a Description: " new_description

file_path="./data/$item_number.item"

if [ ! -f $file_path ]; then
    echo "ERROR: item not found"
    exit 1
fi

export item_number

simple_name_export="export simple_name\n"
item_name_export="export item_name\n"
item_number_export="export item_number\n"
current_quantity_export="export current_quantity\n"
max_quantity_export="export max_quantity\n"
description_export="export description\n"

read_lines="read simple_name item_name\nread current_quantity max_quantity\nread description\n"
export_vars="$simple_name_export$item_name_export$item_number_export$current_quantity_export$max_quantity_export$description_export"

echo -e "$read_lines$export_vars" > file_read.bash
chmod 700 file_read.bash
source ./file_read.bash < $file_path

if [ ! -z $new_simple_name ]; then
    simple_name=$new_simple_name
fi

if [ ! -z "$new_item_name" ]; then
    item_name=$new_item_name
fi

if [ ! -z $new_current_quantity ]; then
    current_quantity=$new_current_quantity
fi

if [ ! -z $new_max_quantity ]; then
    max_quantity=$new_max_quantity
fi

if [ ! -z "$new_description" ]; then
    description=$new_description
fi

echo -e "$simple_name $item_name\n$current_quantity $max_quantity\n$description" > $file_path
echo -e "UPDATED: $simple_name - `date`" >> ./data/queries.log

rm ./file_read.bash
