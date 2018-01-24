#!/bin/bash

PROFILE=

if [ $# -ne 2 ]
then
	echo "Usage: $0 <pfns file> <interval>"
	exit 1
fi

pfns_file=$1
interval=$2

if [ $PROFILE ]; then SECONDS=0; fi
sudo ./pageidle set $pfns_file
if [ $PROFILE ]; then echo "$SECONDS secs for setting pages idle"; fi

sleep $interval

if [ $PROFILE ]; then SECONDS=0; fi
bits=`sudo ./pageidle get $pfns_file`
if [ $PROFILE ]; then echo "$SECONDS secs for getting pages idle"; fi

if [ $PROFILE ]; then SECONDS=0; fi
grep -o "0" <<< $bits | wc -l
if [ $PROFILE ]; then echo "$SECONDS secs for couting working set"; fi
