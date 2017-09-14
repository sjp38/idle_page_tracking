#!/usr/bin/env python

import os
import struct
import sys

if len(sys.argv) < 3:
    print "Usage: %s <pid> <mapping region>" % (sys.argv[0])
    print "\t`mapping region` is same as path field of /proc/pid/maps`"
    print "\tUse `anon` as mapping region for anonymous region"
    exit(1)

PAGE_SHIFT = 12
PAGE_SIZE = 1 << PAGE_SHIFT

def vaof(pid, region):
    vas=[]
    with open("/proc/%s/maps" % pid, 'r') as f:
        for l in f:
            splts = l.split()
            va_range = [int("0x%s" % x, 0) for x in splts[0].split('-')]
            if len(splts) == 5 and region == "anon":
                vas.append(va_range)
            elif len(splts) == 6 and splts[5] == region:
                vas.append(va_range)

    collapsed_vas = [vas[0]]
    for r in vas[1:]:
        if collapsed_vas[-1][1] == r[0]:
            collapsed_vas[-1][1] = r[1]
        else:
            collapsed_vas.append(r)
    return collapsed_vas

def pr_vas(vas):
    for r in vas:
        print "0x%x-0x%x" % (r[0], r[1])

def PAGEMAP_PFN(ent):
    return ent & ((1 << 55) - 1)


pid = sys.argv[1]
region = sys.argv[2]

vas = vaof(pid, region)

with open("/proc/%s/pagemap" % pid, 'rb') as f:
    for r in vas:
        start_va = r[0]
        end_va = r[1]

        vaddr = start_va
        while vaddr <= end_va:
            offset = vaddr >> (PAGE_SHIFT - 3)
            vaddr += PAGE_SIZE

            f.seek(offset, 0)
            ent = f.read(8)
            ent = struct.unpack("Q", ent)[0]
            pfn = PAGEMAP_PFN(ent)
            if pfn == 0:
                continue
            print "%d" % pfn
