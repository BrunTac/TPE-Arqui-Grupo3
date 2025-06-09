#include <stdint.h>
#include <videoDriver.h>

#define REGISTER_AMOUNT 19


void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

char * registerNamesCall[] = {"Instruction pointer: ", "CS: ", "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSP: ", 
			"RBP: ", "RDI: ", "RSI: ", "R8: ", "R9: ", "R10: ", "R11: ", "R12: ", 
			"R13: ", "R14: ", "R15: ", "RFLAGS: " };
			
void printRegistersCall(uint64_t * registers){

    for(int i = 0 ; i < REGISTER_AMOUNT ; i++) {
		prints(registerNamesCall[i], WHITE, BLACK); 
		printHex(registers[i], WHITE, BLACK);
		print_newline();
	}

}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

int strcmp(const char * s1, const char * s2){

	int r1 = 0;
	int r2 = 0;

	while(s1[r1] != '\0' && s2[r2] != '\0'){
		if(s1[r1] != s2[r2]){
			return s1[r1] - s2[r2];
		}
		r1++;
		r2++;
	}

	return s1[r1] - s2[r2];
}

 void strcpy(char * destination, const char * source){

	int w = 0;

	while(source[w] != '\0'){
		destination[w] = source[w];
		w++;
	}

	destination[w] = '\0';
 }

uint64_t strlen(const char *str) {
    uint64_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}
