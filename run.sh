#!/bin/bash
echo "building..."
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
	qemu-system-i386 -cdrom os.iso -serial file:com1.out -device rtl8139,mac="a0:51:0b:e4:cc:aa" -m 1024 -s -S 2>/dev/null &
	cd src
	gdb kernel.bin -ex 'target remote localhost:1234'
else
	qemu-system-i386 -cdrom os.iso -serial file:com1.out -device rtl8139,mac="a0:51:0b:e4:cc:aa" -enable-kvm -m 1024 -hda disk.img
fi
#bochs -f bochs.bxrc -q &
