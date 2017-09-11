#!/usr/bin/env python

import os
import struct
import sys

if len(sys.argv) < 3:
    print "Usage: %s <pid> <vaddr>" % (sys.argv[0])
    exit(1)

PAGE_SHIFT = 12
PAGE_SIZE = 1 << PAGE_SHIFT

pid = sys.argv[1]
vaddrs = [int(x, 0) for x in sys.argv[2:]]

def PAGEMAP_PFN(ent):
    return ent & ((1 << 55) - 1)


with open("/proc/%s/pagemap" % pid, 'rb') as f:
    for vaddr in vaddrs:
        offset = vaddr >> (PAGE_SHIFT - 3)
        f.seek(offset, 0)
        ent = f.read(8)
        ent = struct.unpack("<Q", ent)[0]
        print "%x" % PAGEMAP_PFN(ent)
