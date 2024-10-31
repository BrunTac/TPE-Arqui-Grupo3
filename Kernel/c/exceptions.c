#include <videoDriver.h>
#include <lib.h>

#define ZERO_EXCEPTION_ID 0
#define INVALID_OP_CODE 6
#define REGISTER_AMOUNT 19

char * registerNamesExc[] = {"Instruction pointer: ", "CS: ", "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSP: ", 
			"RBP: ", "RDI: ", "RSI: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ", 
			"R13: ", "R14: ", "R15: ", "RFLAGS: " };

void printRegistersExc(uint64_t * registers){

    for(int i = 0 ; i < REGISTER_AMOUNT ; i++) {
		prints(registerNamesExc[i], RED, BLACK); 
		printHex(registers[i], RED, BLACK);
		print_newline();
	}

}

void exceptionDispatcher(int exception, uint64_t * registers) {
	switch(exception) {
		case ZERO_EXCEPTION_ID:
			prints("Exception: zero-division detected\n", RED, BLACK);
			break;
		case INVALID_OP_CODE:
			prints("Exception: invalid operation code detected\n", RED, BLACK);
	}
	printRegistersExc(registers);
}


