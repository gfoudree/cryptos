#!/bin/bash

make_fat_disk() {
	rm -f disk.img
	dd if=/dev/zero of=disk.img bs=1M count=64
	mkfs.msdos -F 16 -n "CryptOS" disk.img

	echo "Hello!" > /tmp/README.TXT
	mcopy -i disk.img /tmp/README.TXT ::/

	rm /tmp/README.TXT
}

echo "building..."
rm -f src/kernel.bin
rm -f os.iso

make

if [ $? -ne 0 ]
then
	exit
fi

make_fat_disk

if grub-file --is-x86-multiboot src/kernel.bin; then
	  echo multiboot confirmed
else
	    echo the file is not multiboot
fi

if [ "$1" == "debug" ]
then
	qemu-system-i386 -boot d -cdrom os.iso -serial file:com1.out -m 1024 -hda disk.img -s -S 2>/dev/null & 
	cd src
	gdb kernel.bin -ex 'target remote localhost:1234'
else
	qemu-system-i386 -boot d -cdrom os.iso -serial file:com1.out -m 1024 -hda disk.img
fi
#bochs -f bochs.bxrc -q &
