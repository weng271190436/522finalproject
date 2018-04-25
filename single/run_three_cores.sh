#!/bin/bash
taskset -c 1 bash file1.sh &
taskset -c 2 bash file2.sh &
taskset -c 3 bash file3.sh &
taskset -c 1 bash file4.sh &
taskset -c 2 bash file5.sh &
taskset -c 3 bash file6.sh &
exit 0
