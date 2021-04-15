all:
	rm -f os.iso

	$(MAKE) -C src
	$(MAKE) -C isodir/modules
	
	cp src/kernel.bin isodir/boot/kernel.bin
	grub-mkrescue -o os.iso isodir
clean:
	$(MAKE) clean -C src
	$(MAKE) clean -C isodir/modules
	rm -f isodir/boot/kernel.bin
