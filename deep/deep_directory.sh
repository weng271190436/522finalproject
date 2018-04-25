#!/bin/bash
START_TIME=$(date +%s%N)
start=1

for i in {1,2}
do
mkdir -p $i
for j in {1,2}
do
mkdir -p $i/$j
done
done

./monitor_flat_directory_structure 1 2 1/1 1/2 2/1 2/2


ELAPSED_TIME=$((($(date +%s%N) - $START_TIME)/1000000))
echo "Time taken: $ELAPSED_TIME ms"
exit 0
