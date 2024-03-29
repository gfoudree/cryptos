[BITS 32]
global start, gdt_flush, disable_pic, idt_load, syscall_gate, enable_a20
extern _kmain, code, bss, end, syscall_handler, idtp, fault_handler, irq_handler, page_translate_noreloc

PAGE_PRESENT equ 1
PAGE_WRITABLE equ 2
PAGE_USER equ 4
PAGE_WRITE_THROUGH equ 8
PAGE_CACHE_DISABLE equ 16
PAGE_ACCESSED equ 32
PAGE_DIRTY equ 64
PAGE_4MB equ 128

PAGE_RW_4MB equ PAGE_PRESENT | PAGE_WRITABLE | PAGE_4MB

ALIN     equ 1<<0
MEMINFO  equ 1<<1
FLAGS    equ ALIN | MEMINFO
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

KERNEL_VBASE equ 0xC0000000 ;Load kernel at this vaddr
KERNEL_PAGE_INDEX equ (KERNEL_VBASE >> 22) ;Get page directory entry index

%macro isr_handler 1
global _isr%1
_isr%1:
  cli
  push byte 0
  push byte %1
  jmp isr_common_stub
%endmacro

%macro irq_handler 2
global _irq%1
_irq%1:
  cli
  push byte 0
  push byte %2
  jmp irq_common_stub
%endmacro

[SECTION .data]
ALIGN 0x1000
page_directory: ;We only have 1 table with 4MB pages!

  times (0x1000) dd 0 ; Empty page directory

ALIGN 4
[SECTION .multiboot]
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

    dd code
    dd bss
    dd end
    dd start

[SECTION .text]
global init
init:
  ;http://www.rcollins.org/ddj/May96/
  ; Setup identity paging (virt -> phys 1:1)
  push (page_directory - KERNEL_VBASE) ; Calculate offset address
  push PAGE_RW_4MB
  push 0    ;Physical Addr
  push 0    ;Virtual Addr
  mov ecx, (page_translate_noreloc - KERNEL_VBASE) ;Map 0x00000000 - 0x00400000 (First 4 MB) to 0x00000000 - 0x00400000
  call ecx
  
  ; Setup kernel code region 4MB
  push (page_directory - KERNEL_VBASE) ; Calculate offset address
  push PAGE_RW_4MB
  push 0            ;Physical Addr
  push KERNEL_VBASE   ;Virtual Addr
  mov ecx, (page_translate_noreloc - KERNEL_VBASE) ;Map 0xC0000000 - 0xC0400000 (Kernel 4 MB) to 0x00000000 - 0x00400000
  call ecx

  mov ecx, (page_directory - KERNEL_VBASE) ;Copy PHYSICAL addr of page table
  mov cr3, ecx

  mov ecx, cr4
  or ecx, 0x00000010 ;Setup 4MB pages
  mov cr4, ecx

  mov ecx, cr0
  or ecx, 0x80000000 ;Enable paging by setting bit
  mov cr0, ecx

  lea ecx, [start] ;Update EIP register to higher-half addresses
  jmp ecx

start:
  ;mov dword [page_directory], 0 ;We can unmap this 0x0->0x0 mapping if we want
  ;invlpg [0] ;Invalid the page

  
  mov esp, _sys_stack ;Setup 16k stack pointer

  mov eax, end
  add eax, 512 ;Pass heap pointer in eax, heap = end + 512
  push eax

  add ebx, KERNEL_VBASE ;Make the multiboot header a virtual addr
  push ebx ;Push multiboot header

  call _kmain
  jmp $

gdt_flush:
	mov eax, [esp+4] ;Get param on stack
	lgdt [eax]
  jmp 0x08:reload_segments ;Can't set CS directly, let's do it with a jmp!

reload_segments:
  ;0x10 points to the new data selector
	mov ax, 0x10 ;0x10 = 0b10000 = index 2 (data), gdt = 0, priv = 0
	mov ds, ax ;Setup segment registers with our new GDT entries
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret

disable_pic:
  mov al, 0xff
  out 0xa1, al
  out 0x21, al

enable_a20:
  push ax
  mov al, 2
  out 0x92, al
  pop ax
  ret

syscall_gate:
  cli
  push eax
  push ebx
  push ecx
  push edx
  mov eax, syscall_handler
  call eax
  add esp, 16 ;Restore stack
  sti
  iret

idt_load:
  lidt [idtp]
  ret

;Create ISR 0-31 with a macro
%assign i 0
%rep 32
  isr_handler i
%assign i i+1
%endrep

isr_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp
	push eax
	mov eax, fault_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
  sti
	iret

%assign i 0
%rep 16
  irq_handler i,(i+32)
%assign i i+1
%endrep

irq_common_stub:
	pusha
	push ds
	push es
	push fs
	push gs

	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov eax, esp

	push eax
	mov eax, irq_handler
	call eax
	pop eax

	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8 ;Restore stack for the two items pushed
  sti
	iret

[SECTION .bss]
ALIGN 4
    resb 16384
_sys_stack:
