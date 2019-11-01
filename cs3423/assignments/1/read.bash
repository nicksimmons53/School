#!/bin/bash

read -p "Enter An Item Number: " item_number

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

read_lines="read simple_name item_name\nread current_quantity max_quantity\nread description\n"
export_vars="$simple_name_export$item_name_export$item_number_export$current_quantity_export$max_quantity_export"

echo -e "$read_lines$export_vars" > file_read.bash
chmod 700 file_read.bash
source ./file_read.bash < $file_path

echo "Item name: $item_name"
echo "Simple name: $simple_name"
echo "Item Number: $item_number"
echo "Qty: $current_quantity/$max_quantity"
echo "Description: $description"

rm ./file_read.bash
