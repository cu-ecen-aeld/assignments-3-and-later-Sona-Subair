#!/bin/sh

#Checking if the number of arguments is not equal to two
if [ $# -ne 2 ]
    then
        echo "Echo expected two arguments";
        exit 1
fi
filedir=$1
string=$2
count=0
string_count=0

#Checking if the path provided is a directory
if [ ! -d "$filedir" ];
    then
        echo "Not a file directory"
        exit 1
fi

count=$(find "$filedir" -type f | wc -l)
string_count=$(grep -r $string $filedir | wc -l)   

echo "The number of files are ${count} and the number of matching lines are ${string_count}"
exit 0
