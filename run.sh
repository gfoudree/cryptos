#!/bin/bash
echo "building..."
#alias grub-mkrescue=grub2-mkrescue
rm -f src/kernel.bin
rm -f os.iso

make

if [ $? -ne 0 ]
then
	exit
fi

if grub-file --is-x86-multiboot src/kernel.bin; then
	  echo multiboot confirmed
else
	    echo the file is not multiboot
fi

if [ "$1" == "debug" ]
then
	qemu-system-i386 -cdrom os.iso -serial file:com1.out -m 1024 -s -S 2>/dev/null & 
	cd src
	gdb kernel.bin -ex 'target remote localhost:1234'
else
	qemu-system-i386 -cdrom os.iso -serial file:com1.out -m 1024
fi
#bochs -f bochs.bxrc -q &
