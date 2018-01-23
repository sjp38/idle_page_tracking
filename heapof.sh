#!/bin/bash

if [ $# -ne 1 ]
then
	echo "Usage: $0 <pid>"
	exit 1
fi

BINPATH=`dirname $0`
mv $BINPATH

PID=$1

./pfnofmap.py $1 "[heap]"
