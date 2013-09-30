#!/bin/bash
echo "building..."

rm *.iso
cd src
make clean
make
cd ..
cp src/kernel.bin isodir/boot/kernel.bin
grub-mkrescue -o os.iso isodir
#qemu -cdrom os.iso
bochs -f bochs.bxrc
