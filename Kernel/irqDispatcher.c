#include <time.h>
#include <stdint.h>


extern uint8_t keyboard_read();
static void int_20();
static void int_16();

void irqDispatcher(uint64_t irq) {
	
	
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_16();
			break;
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_16() {
	uint8_t scancode = keyboard_read();
	char key = 
}



