#!/bin/sh
#Requires the docker image is built. If not, type
#docker build -t devenv .

echo "building..."

docker run --rm -v "$PWD":/tmp/myos -w /tmp/myos devenv bash -c 'make clean;make'

qemu-system-i386 -kernel src/kernel.bin -serial file:com1.out
#bochs -f bochs.bxrc -q &
