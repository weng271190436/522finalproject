#!/bin/bash
START_TIME=$(date +%s%N)
start=1

mkdir {1..2000}
./monitor_flat_directory_structure {1..2000}
for num in {1..2000}
do
touch ./$num/testfile
rm ./$num/testfile

done
ELAPSED_TIME=$((($(date +%s%N) - $START_TIME)/1000000))
rm -rf {1..2000}
echo "Time taken: $ELAPSED_TIME ms"
exit 0
