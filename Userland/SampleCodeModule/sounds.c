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

void appleEaten() {
    sys_beep(360, 1);
}

void levelUp() {
    sys_beep(240, 1);
    sys_beep(360, 1);
    sys_beep(480, 1);
}

void lost() {
    sys_beep(800, 2);
    
    sys_beep(750, 2);
    
    sys_beep(700, 2);

    sys_beep(650, 2);
    
    sys_beep(600, 2);
        
    sys_beep(550, 2);
    
    sys_beep(500, 2);
    
    sys_beep(450, 2);
    
    sys_beep(400, 2);
    
    sys_beep(350, 2);
    
    sys_beep(300, 2);
    
    sys_beep(250, 2);
    
    sys_beep(200, 2);
}
