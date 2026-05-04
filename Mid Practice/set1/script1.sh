#!/bin/bash

num="$1"


if [ "$num" -lt 0 ]; then
    echo "Number is negative"
elif [ "$num" -gt 0 ]; then
    echo "Number is positive"
else
    echo "Number is zero"
fi

