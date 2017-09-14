#!/bin/bash

if [ $# -lt 2 ]
then
	echo "Usage: $0 <pid> <file name> [mapping region]"
	exit 1
fi

pid=$1
pfns_file=$2
mregions="[heap],[stack],anon"
if [ $# -gt 2 ]
then
	mregions=$3
fi

pfns=`sudo ./pfnofmap.py $pid $mregions`
echo $pfns | ./u8lst_txt2bin.py $pfns_file
