#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 <pid> <interval>"
	exit 1
fi

pid=$1
interval=$2

pfns_file="pfns.bin"

heaparea=`sudo ./heapof.py $pid`
pfns=`sudo ./pfnof.py $pid $heaparea`
echo $pfns | ./txtlist2bin.py $pfns_file

sudo ./pageidle set $pfns_file
sleep $interval
bits=`sudo ./pageidle get $pfns_file`
grep -o "0" <<< $bits | wc -l
