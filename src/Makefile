CC = gcc
CFLAGS = -O0 -g -ffreestanding -nostdlib  -I ../include
ASM = nasm
ASMFLAGS = -f elf
LD = ld
LDFLAGS = -T link.ld -nostdlib -nostdinc

all: kernel.bin

kernel.bin : loader.o main.o screen.o ioports.o gdt.o serial.o libc.o sysctl.o idt.o irq.o isrs.o
	$(CC) $(LDFLAGS) loader.o main.o screen.o ioports.o gdt.o serial.o libc.o sysctl.o irq.o idt.o isrs.o -o kernel.bin

loader.o : loader.asm
	$(ASM) $(ASMFLAGS) loader.asm -o loader.o

main.o : main.c
	$(CC) $(CFLAGS) main.c -c -o main.o

screen.o : screen.c
	$(CC) $(CFLAGS) screen.c -c -o screen.o

ioports.o : ioports.c
	$(CC) $(CFLAGS) ioports.c -c -o ioports.o

gdt.o : gdt.c
	$(CC) $(CFLAGS) gdt.c -c -o gdt.o

serial.o : serial.c
	$(CC) $(CFLAGS) serial.c -c -o serial.o

libc.o : libc.c
	$(CC) $(CFLAGS) libc.c -c -o libc.o

sysctl.o : sysctl.c
	$(CC) $(CFLAGS) sysctl.c -c -o sysctl.o

idt.o : idt.c
	$(CC) $(CFLAGS) idt.c -c -o idt.o

irq.o : irq.c
	$(CC) $(CFLAGS) irq.c -c -o irq.o

isrs.o : isrs.c
	$(CC) $(CFLAGS) isrs.c -c -o isrs.o

keyboard.o : keyboard.c
	$(CC) $(CFLAGS) keyboard.c -c -o keyboard.o

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
