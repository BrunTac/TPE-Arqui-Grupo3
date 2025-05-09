include Makefile.inc

KERNELELF=kernel.elf
KERNEL=kernel.bin
SOURCES=$(wildcard c/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o) font.o 		# agregamos font.o a los objetos, para que se genere en el make
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL) $(KERNELELF)

$(KERNEL): $(KERNELELF)
	$(OBJCOPY) -O binary $(KERNELELF) $(KERNEL)


%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

# Regla para generar font.o desde font.psf usando objcopy
font.o: font.psf
	objcopy -O elf64-x86-64 -B i386 -I binary font.psf font.o

$(KERNELELF): $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) font.o
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNELELF) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS) font.o

clean:
	rm -rf asm/*.o c/*.o *.bin

.PHONY: all clean
