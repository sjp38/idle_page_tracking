#!/usr/bin/env python

import os
import struct
import sys

if len(sys.argv) < 3:
    print "Usage: %s <pid> <start vaddr>-<end vaddr>" % (sys.argv[0])
    exit(1)

PAGE_SHIFT = 12
PAGE_SIZE = 1 << PAGE_SHIFT

pid = sys.argv[1]
va_range = sys.argv[2].split('-')
start_va = int(va_range[0], 0)
end_va = int(va_range[1], 0)

def PAGEMAP_PFN(ent):
    return ent & ((1 << 55) - 1)


with open("/proc/%s/pagemap" % pid, 'rb') as f:
    vaddr = start_va
    while vaddr <= end_va:
        offset = vaddr >> (PAGE_SHIFT - 3)
        vaddr += PAGE_SIZE

        f.seek(offset, 0)
        ent = f.read(8)
        ent = struct.unpack("<Q", ent)[0]
        pfn = PAGEMAP_PFN(ent)
        if pfn == 0:
            continue
        print "%d" % pfn
