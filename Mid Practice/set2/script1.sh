#! /bin/bash

num="$1"

if (( num % 2 == 0 )); then
    echo "Even"
else
    echo "Odd"
fi

