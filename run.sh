#!/bin/sh
#Requires the docker image is built. If not, type
#docker build -t devenv .
#ALSO: SELinux seems to cause problems at times so just do setenforce 0
echo "building..."

rm -f src/kernel.bin
rm -f os.iso

sudo docker run --rm -v "$PWD":/tmp/myos -w /tmp/myos devenv make

if [[ $? -ne 0 ]]
then
	exit
fi

cp src/kernel.bin isodir/boot/kernel.bin

grub-mkrescue -o os.iso isodir

if [[ "$1" == "debug" ]]
then
	qemu-system-i386 -cdrom os.iso -serial file:com1.out -m 4096 -s -S 2>/dev/null & 
	cd src
	gdb kernel.bin -ex 'target remote localhost:1234'
else
	qemu-system-i386 -cdrom os.iso -serial file:com1.out -m 4096
fi
#bochs -f bochs.bxrc -q &
