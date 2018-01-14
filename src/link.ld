OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(init)

SECTIONS {
  . = 0xC0100000;

  .text : AT(ADDR(.text) - 0xC0000000){
    code = .;
    *(.multiboot)
    *(.text)
    *(.rodata)
    . = ALIGN(4096);
  }

  .data : AT(ADDR(.data) - 0xC0000000) {
    *(.data)
    . = ALIGN(4096);
  }

  .bss : AT(ADDR(.bss) - 0xC0000000) {
    bss = .;
    *(COMMON)
    *(.bss)
    . = ALIGN(4096);
  }

  end = .;
}
