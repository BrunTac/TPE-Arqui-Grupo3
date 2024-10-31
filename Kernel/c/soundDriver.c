 #include <stdint.h>
 #include <time.h>

extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);

 //Play sound using built-in speaker
static void play_sound(uint32_t frequency) {
 	uint32_t divisor;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	divisor = 1193180 / frequency;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (divisor) );
 	outb(0x42, (uint8_t) (divisor >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }
 
 //make it shut up
 static void nosound() {
 	uint8_t tmp = inb(0x61) & 0xFC;
     
 	outb(0x61, tmp);
 }

 //Make a beep
 void beep() {
 	 play_sound(1000);
 	 sleep(70);
 	 nosound();
          //set_PIT_2(old_frequency);
 }