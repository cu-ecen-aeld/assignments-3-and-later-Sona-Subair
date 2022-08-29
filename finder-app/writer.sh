#!/bin/bash

#Checking if the number of arguments is less than two
if [ $# -ne 2 ]
    then
        echo "Expected two arguments"
        exit 1
fi        
path=$1
content=$2

#Splitting the path provided to directory name ans filename
file_dir=$(dirname "$path")
file_name=$(basename "$path")

#Create directory if it doesn't exist
if [ ! -e "$file_dir" ]
    then
        mkdir "$file_dir"
fi

touch "$path"
echo "$content" > "$path"

#Check if the file was created successfully
if [ ! -e "$path" ];
    then
        echo "File not created successfully"
        exit 1
fi        
exit 0