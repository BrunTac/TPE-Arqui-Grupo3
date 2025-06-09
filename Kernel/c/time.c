#include <time.h>
#include <interrupts.h>
#include <keyboard.h>
#include <videoDriver.h>
#include <scheduler.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	_sti();
	if(ticks % 50 == 0){
		emptyBuffer();
		clearLastPressed();
	}
}

void sleep(int period) {

    int start = ticks_elapsed();
    while(ticks_elapsed() - start < period) _hlt();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
