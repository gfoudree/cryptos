CC = clang 
CFLAGS = -target i386-pc-none-elf -O0 -ffreestanding -nostdlib  -I ../include -Wall -Wno-pointer-sign -Wno-unused-variable -Wno-unused-function -std=c99 -g3 -fcommon
ASM = nasm
ASMFLAGS = -f elf
LDFLAGS = -T link.ld -nostdlib -nostdinc -g3
TARGET = kernel.bin

all : $(TARGET)

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS)

loader.o : loader.asm
	$(ASM) $(ASMFLAGS) -o $@ $^


$(TARGET) : loader.o main.o screen.o ioports.o gdt.o serial.o libc.o sysctl.o idt.o irq.o isrs.o keyboard.o kernel.o cmos.o timer.o mm.o pci.o ata.o e1000.o
	$(CC) -target i386-pc-none-elf fs/diskio.c fs/ff.c fs/ffsystem.c $(CFLAGS) $^ -o $(TARGET) -T link.ld

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
