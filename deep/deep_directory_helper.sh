#!/bin/bash
START_TIME=$(date +%s%N)
start=1

for i in {1,2}
do
touch $i/testfile
rm $i/testfile
for j in {1,2}
do
touch $i/$j/testfile
rm $i/$j/testfile
done
done

ELAPSED_TIME=$((($(date +%s%N) - $START_TIME)/1000000))
echo "Time taken: $ELAPSED_TIME ms"
exit 0
