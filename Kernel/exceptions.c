#include <videoDriver.h>

#define REGISTER_AMOUNT 18
#define ZERO_EXCEPTION_ID 0
#define INVALID_OP_CODE 6

char * registerNames[] = {"Instruction pointer: ", "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RBP: ", 
			"RSI: ", "RDI: ", "RSP: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ", 
			"R13: ", "R14: ", "R15: ", "RFLAGS: " };

void printRegisters(const uint64_t * registers) { // aca se llamaria un syscall de registros??
	for(int i = 0 ; i < REGISTER_AMOUNT ; i++) {
		prints(registerNames[i]); 
		printHex(registers[i]);
		print('\n');
	}
}

void exceptionDispatcher(int exception, const uint64_t * registers) {
	switch(exception) {
		case ZERO_EXCEPTION_ID:
			prints("Exception: zero-division detected\n");
			break;
		case INVALID_OP_CODE:
			prints("Exception: invalid operation code detected\n");
	}
	printRegisters(registers);
}


