#!/usr/bin/env python

import sys

if len(sys.argv) < 2:
    print "Usage: %s <pid>" % sys.argv[0]
    exit(1)

pid = sys.argv[1]

with open("/proc/%s/maps" % pid, 'r') as f:
    for l in f:
        splts = l.split()
        if splts[-1] == "[heap]":
            va_range = splts[0].split('-')
            print "0x%s-0x%s" % (va_range[0], va_range[1])
