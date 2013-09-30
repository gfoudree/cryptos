CC = gcc
CFLAGS = -O -ffreestanding -fstrength-reduce -fomit-frame-pointer -nostdinc -fno-builtin -nostdlib -m32 -I ../include -fno-strict-aliasing -fno-common -fno-stack-protector
ASM = nasm 
ASMFLAGS = -f elf
LD = ld
LDFLAGS = -T link.ld -melf_i386

all: kernel.bin

kernel.bin : loader.o main.o screen.o ioports.o gdt.o
	$(LD) $(LDFLAGS) loader.o main.o screen.o ioports.o gdt.o -o kernel.bin

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
clean:
	rm *.o
	rm *.bin

