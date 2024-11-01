#include <videoDriver.h>
#include <stdio.h>

#define SIZE 16

const uint16_t DEFAULT_WIDTH = 8;
const uint16_t DEFAULT_HEIGHT = 16;
const uint16_t DEFAULT_SCALE = 1;

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

unsigned int scale = DEFAULT_SCALE;

void updateSize(int size) {

	scale = size;

}

void putPixel(Color color, uint64_t x, uint64_t y) {
    
    if (x >= VBE_mode_info->width || y >= VBE_mode_info->height){
		return;
	}
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * (VBE_mode_info->bpp/8)) + (y * (VBE_mode_info->pitch));
    framebuffer[offset]     =  (color) & 0xFF;
    framebuffer[offset+1]   =  (color >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (color >> 16) & 0xFF;
}

// inicio del bitmap
extern unsigned char _binary_font_psf_start[];

unsigned char *font = _binary_font_psf_start;

//coordenadas de donde escribir el proximo char
uint16_t current_X = 0;
uint16_t current_Y = 0;


void putCharInPos(unsigned char c, int x, int y, Color fgcolor, Color bgcolor){
	int putX, putY;
	int mask[8]={1,2,4,8,16,32,64,128};
	unsigned char * symbol = font + 4 + (int) c * 16;

	if (x >= VBE_mode_info->width) {
		x = 0;
        	if (y + DEFAULT_HEIGHT > VBE_mode_info->height) {
				y -= DEFAULT_HEIGHT;
            	scrollUp();
        	} else {
            		y += DEFAULT_HEIGHT;
        	}
    	}

	for(putY = 0 ; putY < DEFAULT_HEIGHT ; putY++){
		for(putX = 0 ; putX < DEFAULT_WIDTH ; putX++){
			putPixel(symbol[putY] & mask[putX] ? fgcolor : bgcolor, x + (8 - putX), y + putY);
		}
	}	
	x += DEFAULT_WIDTH*scale;
}

void putChar(unsigned char c, Color fgcolor, Color bgcolor)
{
	int putX, putY;
	int mask[8]={1,2,4,8,16,32,64,128};
	unsigned char * symbol = font + 4 + (int) c * 16;

	if (current_X >= VBE_mode_info->width) {
		current_X = 0;
        	if (current_Y + DEFAULT_HEIGHT > VBE_mode_info->height) {
				current_Y -= DEFAULT_HEIGHT*scale;
            	scrollUp();
        	} else {
            		current_Y += DEFAULT_HEIGHT*scale;
        	}
    	}

	for(putY = 0 ; putY < DEFAULT_HEIGHT ; putY++){
		for(putX = 0 ; putX < DEFAULT_WIDTH ; putX++){
				for(int pixelX = 0 ; pixelX < scale ; pixelX++) {
					for(int pixelY = 0 ; pixelY < scale ; pixelY++)
						putPixel(symbol[putY] & mask[putX] ? fgcolor : bgcolor, current_X + (8 - putX) * scale + pixelX, current_Y + putY * scale + pixelY);
				}
		}
	}	
	current_X += DEFAULT_WIDTH*scale;

}

void printsInPos(const char * str, uint64_t x, uint64_t y, Color font, Color background){
	int auxX = 0;
	int auxY = 0;
	for (int i = 0 ; str[i] != '\0'; i++ ){
        if(x + auxX == VBE_mode_info->width){
			auxX = 0;
			auxY += DEFAULT_HEIGHT;
		}
		putCharInPos(str[i], x + auxX, y + auxY, font, background);
		auxX += DEFAULT_WIDTH;
    }
}

void prints(const char *str, Color fnt, Color bgd){
    for (int i = 0 ; str[i] != '\0'; i++ ){
        print(str[i], fnt, bgd);
    }
}


uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}


void drawSquare(Color color, int x, int y){

	for (size_t i = 0; i < 32; i++)
	{
		for (size_t j = 0; j < 32; j++)
		{
			putPixel(color, x+j, y+i);
		}
		
	}

}

void printHex(const uint64_t num, Color fnt, Color bgd) {

	static char buffer[64] = { '0' };

	uintToBase(num, buffer, 16);
	prints(buffer, BLACK, WHITE);
}

void clear(){

	current_X = VBE_mode_info->width;
	current_Y = VBE_mode_info->height;
	while(current_X != 0 || current_Y != 0){
		print_backspace(BLACK, BLACK);
	}

}

void print(const char c, Color fnt, Color bgd){
	switch (c) {
        case '\n':
            print_newline();
        	break;
        case '\b':
            print_backspace(fnt, bgd);
        	break;
        case '\0':
        	break;
        default:

            putChar(c, fnt , bgd);
        	break;
    }
}

void scrollUp (){
    Color* pixel, *next;
    for (int i = 0 ; i < current_Y + DEFAULT_HEIGHT*scale ; i++){
        for (int j = 0 ; j < VBE_mode_info->width ; j++){
            pixel = (Color *) getPixelPtr(j,i);
            next = (Color *) getPixelPtr(j, i+ DEFAULT_HEIGHT*scale);
            *pixel = *next;
        }
    }
}

uint32_t* getPixelPtr(uint16_t x, uint16_t y) {
    uint8_t pixelWidth = (VBE_mode_info->bpp/8);
    uint16_t pixelHeight = VBE_mode_info->pitch;  

    uintptr_t pixelPtr = (uintptr_t)(VBE_mode_info->framebuffer) + (x * pixelWidth) + (y * pixelHeight);
    return (uint32_t*)pixelPtr;
}


void print_newline(){
    current_X = 0;
    current_Y += DEFAULT_HEIGHT * scale;

    if (current_Y + DEFAULT_HEIGHT * scale > VBE_mode_info->height){
        current_Y -= DEFAULT_HEIGHT * scale;
        scrollUp();
    }
}

void print_backspace(Color fnt, Color bgd){
    if (current_X >= DEFAULT_WIDTH * scale){
	current_X -= DEFAULT_WIDTH * scale;
    } else {
        current_X = VBE_mode_info->width - DEFAULT_WIDTH * scale;
		current_Y -= DEFAULT_HEIGHT * scale;
    }
    putChar(' ', fnt, bgd);
    current_X -= DEFAULT_WIDTH * scale;
}

uint64_t getHeight(){
	return (uint64_t)(VBE_mode_info->height);
}

uint64_t getWidth(){
	return (uint64_t)(VBE_mode_info->width);
}

uint64_t getFontWidth(){
	return DEFAULT_WIDTH * scale;
}