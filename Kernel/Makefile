include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard c/*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:.c=.o) font.o 		# agregamos font.o a los objetos, para que se genere en el make
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

# Define which files to use for each target before any compilation
ifeq ($(MAKECMDGOALS),buddy)
    GCCFLAGS += -DUSE_BUDDY_MM
    override SOURCES := $(filter-out c/chosenMM.c,$(wildcard c/*.c))
else
    GCCFLAGS += -DUSE_NORMAL_MM
    override SOURCES := $(filter-out c/buddyMM.c,$(wildcard c/*.c))
endif

override OBJECTS := $(SOURCES:.c=.o) font.o

all: $(KERNEL)

buddy: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM) font.o
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o  kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)


%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

# Regla para generar font.o desde font.psf usando objcopy
font.o: font.psf
	objcopy -O elf64-x86-64 -B i386 -I binary font.psf font.o

clean:
	rm -rf asm/*.o c/*.o *.bin

.PHONY: all clean
