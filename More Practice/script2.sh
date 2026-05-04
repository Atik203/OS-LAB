#!/bin/bash

for file in *.txt; do
    x=$(head -n "$1" "$file" | tail -n 1 | grep -w "$2" | wc -l)    # check if the nth line contains the string
    if [ "$x" -eq 1 ]; then
        rm -f "$file"
        echo "File $file deleted"
    fi

    # or... you can use -q option of grep to suppress output
    # notice there is no third bracket here, because grep -q returns true/false directly
    # if head -n "$1" "$file" | tail -n 1 | grep -q -w "$2"; then     
    #     rm -f "$file"
    #     echo "File $file deleted"
    # fi
done
