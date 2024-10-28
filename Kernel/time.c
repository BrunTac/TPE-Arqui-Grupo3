#include <time.h>
#include <interrupts.asm>
#include <keyboard.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	_sti();
	if(ticks % 2 == 0){
		emptyBuffer();
	}
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
