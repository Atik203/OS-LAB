#! /bin/bash

# First Find all the .txt files in the player and it's subdirectories
# -type f: only find files
# -name "*.txt": only find files with .txt extension
find players -type f -name "*.txt" | while read -r file
do
    country=$(head -n 2 "$file" | tail -n 1)
    role=$(head -n 4 "$file" | tail -n 1)
    player=$(head -n 1 "$file")
    mkdir -p "$country/$role"
    mv "$file" "$country/$role/$player.txt"

done

# delete the empty players directory and it's subdirectories
find players -type d -empty -delete