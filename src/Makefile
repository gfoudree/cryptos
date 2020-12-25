CC = clang 
CFLAGS = -target i386-pc-none-elf -O0 -ffreestanding -nostdlib  -I ../include -Wall -Wno-pointer-sign -Wno-unused-variable -Wno-unused-function -std=c99
ASM = nasm
ASMFLAGS = -f elf
LDFLAGS = -T link.ld -nostdlib -nostdinc
TARGET = kernel.bin

all : $(TARGET)

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS) -g3

loader.o : loader.asm
	$(ASM) $(ASMFLAGS) -o $@ $^

$(TARGET) : loader.o main.o screen.o ioports.o gdt.o serial.o libc.o sysctl.o idt.o irq.o isrs.o keyboard.o kernel.o cmos.o timer.o mm.o pci.o ata.o
	$(CC) -target i386-pc-none-elf $(LDFLAGS) $^ -o $(TARGET) -g3

clean:
	rm -f *.o
	rm -f *.bin
	rm -f *.elf
