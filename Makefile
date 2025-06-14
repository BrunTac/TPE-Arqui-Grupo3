
all:  bootloader kernel userland image

buddy: bootloader kernel-buddy userland image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

kernel-buddy:
	cd Kernel; make buddy

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image collections kernel userland all clean
