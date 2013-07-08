CC = gcc
CFLAGS = -ffreestanding -nostdlib -m32 -I ../include
ASM = nasm
ASMFLAGS = -f elf
LD = ld
LDFLAGS = -T link.ld -melf_i386

all: kernel.bin

kernel.bin : loader.o main.o screen.o
	$(LD) $(LDFLAGS) loader.o main.o screen.o -o kernel.bin

loader.o : loader.asm
	$(ASM) $(ASMFLAGS) loader.asm -o loader.o

main.o : main.c
	$(CC) $(CFLAGS) main.c -c -o main.o	

screen.o : screen.c
	$(CC) $(CFLAGS) screen.c -c -o screen.o

clean:
	rm *.o
	rm *.bin

