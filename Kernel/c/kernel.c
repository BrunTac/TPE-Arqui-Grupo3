#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <keyboard.h>
#include <idtLoader.h>
#include <interrupts.h>
#include <processManager.h>
#include <tests.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}

int main()
{	
	_cli();

	load_idt();
	initializeProcessManager();

	/* 
	char *args[] = { "100", NULL };  // or whatever limit you want
    test_mm(1, args);

    // If test_mm loops forever, the kernel will hang there.
    // If test_mm returns due to an error, you can print a message and halt:
    prints("test_mm returned\n", WHITE, BLACK); 
	*/

	char * shell = "shell";
	char * argv[] = {shell};
	createProcess((function) sampleCodeModuleAddress, 1, argv, 1);

	_sti();

	_hlt();

	return 0;
}
