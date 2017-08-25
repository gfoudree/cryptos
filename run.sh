#!/bin/sh
#Requires the docker image is built. If not, type
#docker build -t devenv .
#ALSO: SELinux seems to cause problems at times so just do setenforce 0
echo "building..."

rm src/kernel.bin
docker run --rm -v "$PWD":/tmp/myos -w /tmp/myos devenv make

qemu-system-i386 -kernel src/kernel.bin -serial file:com1.out
#bochs -f bochs.bxrc -q &
