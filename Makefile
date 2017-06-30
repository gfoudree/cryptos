all:
	$(MAKE) -C src

clean:
	$(MAKE) clean -C src

debug:
	qemu-system-i386 -kernel src/kernel.bin -serial file:com1.out -s -S
