#!/bin/bash

if [ $# -ne 2 ]
then
	echo $# $0 $1 $2
	echo "Usage: $0 <pid> <file name>"
	exit 1
fi

pid=$1
pfns_file=$2

pfns=`sudo ./pfnofmap.py $pid [heap]`
echo $pfns | ./u8lst_txt2bin.py $pfns_file
