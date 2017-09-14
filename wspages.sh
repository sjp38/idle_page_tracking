#!/bin/bash

if [ $# -lt 2 ]
then
	echo "Usage: $0 <pid> <interval> [mapped regions]"
	exit 1
fi

pid=$1
interval=$2
mregions=$3
pfns_file="pfns.bin"

./gen_pfns_bin.sh $pid $pfns_file $mregions
./wspagesof.sh $pfns_file $interval
