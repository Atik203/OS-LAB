#! /bin/bash

# Check whether a file exists
if [ ! -f "$1" ]; then
	echo "File not found: $1"
	exit 1
fi

# Count the number of lines in the file.
lines=$(wc -l < "$1")
echo "Number of lines in $1: $lines"


