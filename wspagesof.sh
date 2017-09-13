#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 <pfns file> <interval>"
	exit 1
fi

pfns_file=$1
interval=$2

sudo ./pageidle set $pfns_file
sleep $interval
bits=`sudo ./pageidle get $pfns_file`
grep -o "0" <<< $bits | wc -l
