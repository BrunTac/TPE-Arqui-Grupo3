#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <keyboard.h>
#include <idtLoader.h>
#include <interrupts.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
extern Color WHITE;
extern Color BLACK;
static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

typedef enum {
    COLOR_WHITE     = 0xFFFFFF,
    COLOR_BLACK     = 0x000000,
    COLOR_RED       = 0xFF0000,
    COLOR_GREEN     = 0x00FF00,
    COLOR_BLUE      = 0x0000FF,
    COLOR_YELLOW    = 0xFFFF00,
    COLOR_CYAN      = 0x00FFFF,
    COLOR_MAGENTA   = 0xFF00FF,
    COLOR_GRAY      = 0x808080,
    COLOR_LIGHT_GRAY = 0xD3D3D3,
    COLOR_DARK_GRAY = 0xA9A9A9,
    COLOR_ORANGE     = 0xFFA500,
    COLOR_PURPLE    = 0x800080,
    COLOR_BROWN     = 0xA52A2A
} ColorEnum;


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
	load_idt();

	((EntryPoint)sampleCodeModuleAddress)();


	return 0;
}
