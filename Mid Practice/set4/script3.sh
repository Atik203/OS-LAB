#! /bin/bash

# Check whether a number is divisible by both 5 and 11.
read -p "Enter a number: " num

if [ $((num % 5)) -eq 0 ] && [ $((num % 11)) -eq 0 ]; then
	echo "$num is divisible by both 5 and 11."
else
	echo "$num is not divisible by both 5 and 11."
fi



