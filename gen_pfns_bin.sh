#!/bin/bash

if [ $# -ne 2 ]
then
	echo $# $0 $1 $2
	echo "Usage: $0 <pid> <file name>"
	exit 1
fi

pid=$1
pfns_file=$2

heaparea=`sudo ./heapof.py $pid`
pfns=`sudo ./pfnof.py $pid $heaparea`
echo $pfns | ./u8lst_txt2bin.py $pfns_file
