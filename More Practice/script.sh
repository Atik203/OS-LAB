#! /bin/bash

line_num=$1
pattern=$2

for file in *.txt
do
    if head -n "$line_num" "$file" | tail -n 1 | grep -q "$pattern"
    then 
        rm -f "$file"
    fi
    
done