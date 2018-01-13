all:
	$(MAKE) -C src
	$(MAKE) -C isodir/modules

clean:
	$(MAKE) clean -C src
	$(MAKE) clean -C isodir/modules
