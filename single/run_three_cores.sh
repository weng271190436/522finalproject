#!/bin/bash
taskset -c 1 bash core1.sh &
taskset -c 2 bash core2.sh &
taskset -c 3 bash core3.sh &
exit 0
