#include <videoDriver.h>
#include <lib.h>

#define ZERO_EXCEPTION_ID 0
#define INVALID_OP_CODE 6

extern Color RED, BLACK, WHITE;

void exceptionDispatcher(int exception) {
	switch(exception) {
		case ZERO_EXCEPTION_ID:
			prints("Exception: zero-division detected\n", RED, BLACK);
			break;
		case INVALID_OP_CODE:
			prints("Exception: invalid operation code detected\n", RED, BLACK);
	}
	printRegisters();
}


