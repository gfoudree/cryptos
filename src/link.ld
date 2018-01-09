OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(start)

SECTIONS {
  . = 1M;

  .text : {
    code = .;
    *(.multiboot)
    *(.text)
    *(.rodata)
    . = ALIGN(4096);
  }

  .data : {
    *(.data)
    . = ALIGN(4096);
  }

  .bss : {
    bss = .;
    *(.bss)
    . = ALIGN(4096);
  }

  end = .;
}
