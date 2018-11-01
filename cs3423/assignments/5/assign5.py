#!/usr/bin/env python3
import os
import sys
import glob
import re

# Assigning item data to a dictionary
def assign_data(lines):
    item = { }
    item['simple_name'] = lines[0].split( )[0]
    item['item_name'] = ' '.join(str(e) for e in lines[0].split( )[1:])
    item['curr_quantity'] = int(lines[1].split( )[0])
    item['max_quantity'] = int(lines[1].split( )[1])
    item['inventory_calc'] = item['max_quantity']/10
    item['body'] = lines[2].strip("\n")
    return item

def replace_vars(item, template, date, l, r):
    # Replace the matching text in the template
    simple_name = template.replace(l + "simple_name" + r, item['simple_name'])
    item_name = simple_name.replace(l + "item_name" + r, item['item_name'])
    curr_quantity = item_name.replace(l + "current_quantity" + r, str(item['curr_quantity']))
    max_quantity = curr_quantity.replace(l + "max_quantity" + r, str(item['max_quantity']))
    date = max_quantity.replace(l + "date"+ r, date)
    body = date.replace(l + "body" + r, item['body'])
    return body

def assignment5(data_files, args):
    # For each file, read and determine
    for file in data_files:
        try:
            # Open the data file for reading
            with open (file, 'r') as f:
                # Read each line into a list
                lines = f.readlines( )

                # Assign the read data into a dictionary
                item = assign_data(lines)

                # If there is more than 10% inventory, continue to next item
                if item['curr_quantity'] >= item['inventory_calc']:
                    continue

                # Open the template for reading
                t = open(args[2], 'r')
                template = t.read( )
                t.close( )

                l_token = "<<"
                r_token = ">>"
                item_data = replace_vars(item, template, args[3], l_token, r_token)

                # Write the data to the output file
                name = file.split("/")[2].split(".")[0]
                output_path = args[4] + "/" + name + ".out"
                output_file = open(output_path, 'w')
                output_file.write(item_data)
                output_file.close( )

        except IOError:
            print("File is not readable.")

        f.close( )
    sys.exit( )

def extraCredit(data_files, args):
    # For each file, read and determine
    for file in data_files:
        try:
            # Open the data file for reading
            with open (file, 'r') as f:
                # Read each line into a list
                lines = f.readlines( )

                # Assign the read data into a dictionary
                item = assign_data(lines)

                # If there is more than 10% inventory, continue to next item
                if item['curr_quantity'] >= item['inventory_calc']:
                    continue

                # Open the template for reading
                t = open(args[2], 'r')
                template = t.read( )
                t.close( )

                l_token = args[5]
                r_token = args[6]
                item_data = replace_vars(item, template, args[3], l_token, r_token)

                # Write the data to the output file
                name = file.split("/")[2].split(".")[0]
                output_path = args[4] + "/" + name + ".out"
                output_file = open(output_path, 'w')
                output_file.write(item_data)
                output_file.close( )

        except IOError:
            print("File is not readable.")

        f.close( )
    sys.exit( )


# Make the output dir if nonexistent
if (os.path.exists(sys.argv[4]) == False):
    os.mkdir(sys.argv[4])

# Get all data files being read
data_files = glob.glob(sys.argv[1] + "/*")

if len(sys.argv) == 5:
    assignment5(data_files, sys.argv)
elif len(sys.argv) == 7:
    extraCredit(data_files, sys.argv)
else:
    print("Usage: " + sys.argv[0] + " <directory> <template> <date> <directory>")
    sys.exit(1)
