#! /bin/bash

# Ask the user for a file path.
read -p "Enter a file path: " file

# Check whether the file exists.
if [ -e "$file" ]; then
	echo "The file exists."
else
	echo "The file does not exist."
fi

