#!/bin/bash
START_TIME=$(date +%s%N)
for num in {1..2000}
do
touch ./test_directory/file5
rm ./test_directory/file5
done
ELAPSED_TIME=$((($(date +%s%N) - $START_TIME)/1000000))
echo "File 5 time taken: $ELAPSED_TIME ms"
exit 0
