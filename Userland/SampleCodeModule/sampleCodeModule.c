#include <view.h>
#include <libc.h>
#include <stddef.h>
#include <sys_calls.h>

int main() {
	
	initialize();
	sys_createProcess(terminal, 0, NULL, 0);

	return 0;
}