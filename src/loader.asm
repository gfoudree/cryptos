[BITS 32]
global start, gdt_flush, disable_pic, idt_load, syscall_gate, enable_a20

global _isr0, _isr1, _isr2, _isr3, _isr4, _isr5, _isr6, _isr7, _isr8, _isr9, \
  _isr10, _isr11, _isr12, _isr13, _isr14, _isr15, _isr16, _isr17, _isr18, _isr19, \
  _isr20, _isr21, _isr22, _isr23, _isr24, _isr25, _isr26, _isr27, _isr28, _isr29, \
  _isr30, _isr31

global _irq0, _irq1, _irq2, _irq3, _irq4, _irq5, _irq6, _irq7, _irq8, _irq9, \
    _irq10, _irq11, _irq12, _irq13, _irq14, _irq15

extern _kmain, code, bss, end, syscall_handler, idtp, fault_handler, irq_handler

ALIN    equ 1<<0
MEMINFO  equ 1<<1
FLAGS    equ ALIN | MEMINFO
MAGIC    equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

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
start:
  mov esp, _sys_stack ;Setup 16k stack pointer
  push ebx ;Push multiboot header
  mov eax, end
  add eax, 512 ;Pass heap pointer in eax, heap = end + 512

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
  push eax
  push ebx
  push ecx
  push edx
  mov eax, syscall_handler
  call eax
  add esp, 16 ;Restore stack
  ret

idt_load:
  lidt [idtp]
  ret

; 0: Divide by zero exception
_isr0:
	cli
	push byte 0
	push byte 0
	jmp isr_common_stub

; 1: Debug exception
_isr1:
	cli
	push byte 0
	push byte 1
	jmp isr_common_stub

; 2: Non maskable interupt exception
_isr2:
	cli
	push byte 0
	push byte 2
	jmp isr_common_stub

; 3: Breakpoint exception
_isr3:
	cli
	push byte 0
	push byte 3
	jmp isr_common_stub

; 4: Into detected overflow exception
_isr4:
	cli
	push byte 0
	push byte 4
	jmp isr_common_stub

; 5: Out of bounds exception
_isr5:
	cli
	push byte 0
	push byte 5
	jmp isr_common_stub

; 6: Invalid opcode exception
_isr6:
	cli
	push byte 0
	push byte 6
	jmp isr_common_stub

; 7: No coprocessor exception
_isr7:
	cli
	push byte 0
	push byte 7
	jmp isr_common_stub

; 8: Double fault exception
_isr8:
	cli
	push byte 8
	jmp isr_common_stub

; 9: Coprocessor segment overrun exception
_isr9:
	cli
	push byte 0
	push byte 9
	jmp isr_common_stub

; 10: Bad TSS exception
_isr10:
	cli
	push byte 10
	jmp isr_common_stub

; 11: Segment not present
_isr11:
	cli
	push byte 11
	jmp isr_common_stub

; 12: Stack fault exception
_isr12:
	cli
	push byte 12
	jmp isr_common_stub

; 13: General protection fault exception
_isr13:
	cli
	push byte 13
	jmp isr_common_stub

; 14: Page fault exception
_isr14:
	cli
	push byte 14
	jmp isr_common_stub

; 15: Reserved Exception
_isr15:
	cli
	push byte 0
	push byte 15
	jmp isr_common_stub

; 16: Floating point exception
_isr16:
	cli
	push byte 0
	push byte 16
	jmp isr_common_stub

; 17: Alignment check exception
_isr17:
	cli
	push byte 0
	push byte 17
	jmp isr_common_stub

; 18: Machine check exception
_isr18:
	cli
	push byte 0
	push byte 18
	jmp isr_common_stub

; 19: Reserved
_isr19:
	cli
	push byte 0
	push byte 19
	jmp isr_common_stub

; 20: Reserved
_isr20:
	cli
	push byte 0
	push byte 20
	jmp isr_common_stub

; 21: Reserved
_isr21:
	cli
	push byte 0
	push byte 21
	jmp isr_common_stub

; 22: Reserved
_isr22:
	cli
	push byte 0
	push byte 22
	jmp isr_common_stub

; 23: Reserved
_isr23:
	cli
	push byte 0
	push byte 23
	jmp isr_common_stub

; 24: Reserved
_isr24:
	cli
	push byte 0
	push byte 24
	jmp isr_common_stub

; 25: Reserved
_isr25:
	cli
	push byte 0
	push byte 25
	jmp isr_common_stub

; 26: Reserved
_isr26:
	cli
	push byte 0
	push byte 26
	jmp isr_common_stub

; 27: Reserved
_isr27:
	cli
	push byte 0
	push byte 27
	jmp isr_common_stub

; 28: Reserved
_isr28:
	cli
	push byte 0
	push byte 28
	jmp isr_common_stub

; 29: Reserved
_isr29:
	cli
	push byte 0
	push byte 29
	jmp isr_common_stub

; 30: Reserved
_isr30:
	cli
	push byte 0
	push byte 30
	jmp isr_common_stub

; 31: Reserved
_isr31:
	cli
	push byte 0
	push byte 31
	jmp isr_common_stub

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
	iret

; 32: IRQ0
_irq0:
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub

; 33: IRQ1
_irq1:
	cli
	push byte 0
	push byte 33
	jmp irq_common_stub

; 33: IRQ2
_irq2:
	cli
	push byte 0
	push byte 34
	jmp irq_common_stub

; 32: IRQ3
_irq3:
	cli
	push byte 0
	push byte 35
	jmp irq_common_stub

; 32: IRQ4
_irq4:
	cli
	push byte 0
	push byte 36
	jmp irq_common_stub

; 32: IRQ5
_irq5:
	cli
	push byte 0
	push byte 37
	jmp irq_common_stub

; 32: IRQ6
_irq6:
	cli
	push byte 0
	push byte 38
	jmp irq_common_stub

; 32: IRQ7
_irq7:
	cli
	push byte 0
	push byte 39
	jmp irq_common_stub

; 32: IRQ8
_irq8:
	cli
	push byte 0
	push byte 40
	jmp irq_common_stub

; 32: IRQ9
_irq9:
	cli
	push byte 0
	push byte 41
	jmp irq_common_stub

; 32: IRQ10
_irq10:
	cli
	push byte 0
	push byte 42
	jmp irq_common_stub

; 32: IRQ11
_irq11:
	cli
	push byte 0
	push byte 43
	jmp irq_common_stub

; 32: IRQ12
_irq12:
	cli
	push byte 0
	push byte 44
	jmp irq_common_stub

; 32: IRQ13
_irq13:
	cli
	push byte 0
	push byte 45
	jmp irq_common_stub

; 32: IRQ14
_irq14:
	cli
	push byte 0
	push byte 46
	jmp irq_common_stub

; 32: IRQ15
_irq15:
	cli
	push byte 0
	push byte 47
	jmp irq_common_stub

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
	add esp, 8
	iret

[SECTION .bss]
ALIGN 4
    resb 16384
_sys_stack:
