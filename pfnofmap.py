#!/usr/bin/env python

import os
import struct
import sys

if len(sys.argv) < 3:
    print "Usage: %s <pid> <mapping regions>" % (sys.argv[0])
    print "\t`mapping region` is as same as path field of /proc/pid/maps`"
    print "\tUse `anon` as mapping region for anonymous region"
    print "\tMultiple regions can be given using comma as seperator"
    exit(1)

PAGE_SHIFT = 12
PAGE_SIZE = 1 << PAGE_SHIFT

def vaof(pid, regions):
    vas=[]
    with open("/proc/%s/maps" % pid, 'r') as f:
        for l in f:
            splts = l.split()
            va_range = [int("0x%s" % x, 0) for x in splts[0].split('-')]
            if len(splts) == 5 and "anon" in regions:
                vas.append(va_range)
            elif len(splts) == 6 and splts[5] in regions:
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

def IN_LRU(kpflags):
    return (kpflags & (1 << 5)) != 0

def PR_KFLAGS(flags):
    text = ""
    for b in range(0, 26):
        if flags & (1 << b) != 0:
            text += "1"
        else:
            text += "0"
        if b > 0 and b % 4 == 0:
            text += " "
    print text


pid = sys.argv[1]
regions = sys.argv[2].split(',')

vas = vaof(pid, regions)

pmap_path = "/proc/%s/pagemap" % pid
kpflg_path = "/proc/kpageflags"
with open(pmap_path, 'rb') as f, open(kpflg_path, 'rb') as kpflgf:
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

            kpflgf.seek(pfn * 8, 0)
            kpflags = kpflgf.read(8)
            kpflags = struct.unpack("Q", kpflags)[0]
            if not IN_LRU(kpflags):
                continue
            print "%d" % pfn
