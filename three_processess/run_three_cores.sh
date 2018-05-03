#!/bin/bash
taskset -c 1 ./create_delete 1 &
taskset -c 2 ./create_delete 2 &
taskset -c 3 ./create_delete 3 &
exit 0
