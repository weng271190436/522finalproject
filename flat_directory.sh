#!/bin/bash
START_TIME=$(date +%s%N)
for num in {1..2000}
do
touch ./test1/testfile
rm ./test1/testfile
touch ./test2/testfile
rm ./test2/testfile
done
ELAPSED_TIME=$((($(date +%s%N) - $START_TIME)/1000000))
echo "Time taken: $ELAPSED_TIME ms"
exit 0
