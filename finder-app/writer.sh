#!/bin/bash

if [ $# -lt 2 ];
    then
        echo "Expected two arguments"
        exit 1
fi        
path=$1
content=$2

file_dir=$(dirname "$path")
file_name=$(basename "$path")

if [ ! -e "$file_dir" ]
    then
        mkdir "$file_dir"
fi
touch "$path"
echo "$content" > "$path"

if [ ! -e "$path" ];
    then
        echo "File not created successfully"
        exit 1
fi        
exit 0