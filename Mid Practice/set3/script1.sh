#! /bin/bash

num="$1"

factorial=1
for (( i=1; i<=num; i++ )); do
    factorial=$((factorial * i))
done
echo "Factorial of $num is: $factorial"


