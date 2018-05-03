#!/bin/bash


root_path=$(pwd)

make_dir()
{
	echo "$(pwd)" >> "$root_path/args"
	if [[ $1 -gt 0 ]]
	then
		mkdir 1
		cd 1
		make_dir $[$1 - 1]
		cd ..
		mkdir 2
		cd 2
		make_dir $[$1 - 1]
		cd ..
	fi
}

mkdir deep
cd deep
make_dir $1

exit 0
