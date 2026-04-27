#!/bin/bash

num="$1"

if [ -z "$num" ]; then
    echo "Usage: $0 <number>"
    exit 1
fi

if [ "$num" -lt 0 ]; then
    echo "Number is negative"
elif [ "$num" -gt 0 ]; then
    echo "Number is positive"
else
    echo "Number is zero"
fi

