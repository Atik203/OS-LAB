#! /bin/bash

# Count the number of lines in the file provided as the first argument.
if [ $# -ne 1 ]; then
	echo "Usage: $0 <file-name>"
	exit 1
fi

if [ ! -f "$1" ]; then
	echo "File not found: $1"
	exit 1
fi

lines=$(wc -l < "$1")
echo "Number of lines in $1: $lines"


