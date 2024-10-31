#include <time.h>
#include <interrupts.h>
#include <keyboard.h>
#include <videoDriver.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	_sti();
	emptyBuffer();
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
