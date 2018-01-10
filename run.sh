#!/bin/sh
#Requires the docker image is built. If not, type
#docker build -t devenv .
#ALSO: SELinux seems to cause problems at times so just do setenforce 0
echo "building..."

rm -f src/kernel.bin
sudo docker run --rm -v "$PWD":/tmp/myos -w /tmp/myos devenv make
if [[ "$1" == "debug" ]]
then
	qemu-system-i386 -kernel src/kernel.bin -serial file:com1.out -m 4096 -s -S
else
	qemu-system-i386 -kernel src/kernel.bin -serial file:com1.out -m 4096
fi
#bochs -f bochs.bxrc -q &
