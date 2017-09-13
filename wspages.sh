#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 <pid> <interval>"
	exit 1
fi

pid=$1
interval=$2
pfns_file="pfns.bin"

./gen_pfns_bin.sh $pid $pfns_file
./wspagesof.sh $pfns_file $interval
