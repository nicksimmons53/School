#!/bin/bash

# Check args
if [ $# -ne 4 ]; then
    echo "Usage: $0 requires 4 parameters"
    exit 1
fi

# Check if passed dir already exists
if [ -d $4 ]; then
    rm -r ./$4
fi

# Make the dir for output
mkdir ./$4

data_dir=$1
template=$2
date=$3
output_dir=$4

simple_name_export="export simple_name\n"
item_name_export="export item_name\n"
current_quantity_export="export current_quantity\n"
max_quantity_export="export max_quantity\n"
body_export="export body\n"

read_lines="read simple_name item_name\nread current_quantity max_quantity\nread body\n"
export_vars="$simple_name_export$item_name_export$current_quantity_export$max_quantity_export$body_export"

echo -e "$read_lines" > read_file.bash
chmod 700 read_file.bash

for file in $data_dir/*; do
    item=${file##*/}
    item=${item%.*}

    source ./read_file.bash < $file
    inv_count=$((max_quantity / 10))

    if [[ $current_quantity -lt $inv_count ]]; then
        gawk -f generateSED -v "simpName=$simple_name" -v "itemName=$item_name" -v "currQuantity=$current_quantity" -v "maxQuantity=$max_quantity" -v "body=$body" -v "date=$date" $file > gSedFile
        sed -E -f gSedFile assign4.template > $output_dir/$item.out
    fi
done

rm read_file.bash gSedFile
