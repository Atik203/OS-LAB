#! /bin/bash

# loop print 1 to 10
for i in {1..10}
do
  echo $i
done

# another way to loop print 1 to 10
for ((i=1; i<=10; i++))
do
  echo $i
done