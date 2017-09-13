#!/usr/bin/env python

"""Convert list of unsigned long long from text format to binary format"""

import sys
import struct

if len(sys.argv) < 2:
    print "Usage: %s <bin file>" % sys.argv[0]
    exit(1)

binfile = sys.argv[1]

txt = sys.stdin.read()
numbers = [int(n) for n in txt.split()]

with open(binfile, 'wb') as f:
    f.write(struct.pack('Q', len(numbers)))
    for n in numbers:
        f.write(struct.pack('Q', n))
