#!/bin/bash

if [ $# -lt 2 ]
then
	echo "Usage: $0 <pid> <delay> [mapping regions]"
	exit 1
fi

BINDIR=`dirname $0`
pushd $BINDIR > /dev/null

PID=$1
DELAY=$2
MREGIONS=$3

PFNS_FILE="pfns.bin"

PFNF_REUSE_LIM=5
NR_PFNF_REUSE=5
while true
do
	if [ $NR_PFNF_REUSE -eq $PFNF_REUSE_LIM ]
	then
		./gen_pfns_bin.sh $PID $PFNS_FILE $MREGIONS
		NR_PFNF_REUSE=0
	fi
	NR_PFNF_REUSE=$(($NR_PFNF_REUSE + 1))
	WSPAGES=`./wspagesof.sh $PFNS_FILE $DELAY`
	echo "$SECONDS $WSPAGES"
done

popd > /dev/null
