#!/usr/bin/python3

import sys

value = int(sys.argv[1], 16)
print("Page Directory Entry")
print("Value: 0x{:02X}".format(value))

print("Page Table Addr: 0x{:02X}".format(value >> 22))

flags = ''
if value & 1:
    flags += 'Present '
if value & 2:
    flags += 'R/W '
else:
    flags += 'R/O '

if value & 4:
    flags += 'User '
else:
    flags += 'Supervisor '

if value & 8:
    flags += 'Write_Through '

if value & 16:
    flags += 'Cache_Disabled '
if value & 32:
    flags += 'Accessed '
if value & 128:
    flags += '4Mb_Page '
else:
    flags += '4k_Page '

if value & 256:
    flags += 'Ignored '


print("Flags: {}".format(flags))

print("\n\nPage Table Entry:")

print("Physical Page Addr: 0x{:02X}".format(value >> 22))
flags = ''

if value & 1:
    flags += 'Present '
if value & 2:
    flags += 'R/W '
else:
    flags += 'R/O '

if value & 4:
    flags += 'User '
else:
    flags += 'Supervisor '

if value & 8:
    flags += 'Write_Through '

if value & 16:
    flags += 'Cache_Disabled '
if value & 32:
    flags += 'Accessed '

if value & 64:
    flags += 'Dirty '
if value & 256:
    flags += 'Global '

print("Flags: {}".format(flags))
