#! /bin/bash

cd movie_data

for file in *.txt
do
    director=$(tail -n 2 "$file" | head -n 1)
    echo "$file: $director"
    mkdir -p "$director"
    mv "$file" "$director/"
done