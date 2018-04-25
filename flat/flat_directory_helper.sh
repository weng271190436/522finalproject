#!/bin/bash
START_TIME=$(date +%s%N)
for num in {1..2000}
do
touch ./flat/$num/testfile
rm ./flat/$num/testfile

done
ELAPSED_TIME=$((($(date +%s%N) - $START_TIME)/1000000))
echo "Time taken: $ELAPSED_TIME ms"
rm -rf ./flat/{1..2000}
exit 0
