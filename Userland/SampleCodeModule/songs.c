#include <sys_calls.h>

void luffyTune() {
    sys_beep(370, 4);
    sys_beep(392, 3);
    sys_beep(440, 3);
    sys_beep(494, 3);
    sys_beep(554, 3);
    sys_beep(587, 3);
    sys_beep(659, 3);
    sys_beep(740, 3);
    sys_beep(784, 3);
    sys_beep(880, 6);
    sys_beep(1175, 6);
    sys_beep(880, 6);
    sys_beep(784, 2);
    sys_beep(880, 2);
    sys_beep(784, 2);
    sys_beep(740, 14);
    sys_beep(880, 3);
}