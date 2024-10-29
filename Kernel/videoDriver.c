#include <videoDriver.h>
#include <stdio.h>

#define SIZE 16

const uint16_t WIDTH_FONT = 8;
const uint16_t HEIGHT_FONT = 16;

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

Color WHITE = {255, 255, 255};
Color BLACK = {0,0,0};
Color RED = {255, 0, 0};


void printcheck(){
	prints("lol", BLACK, WHITE);
}

void putPixel(Color color, uint64_t x, uint64_t y) {
    
    if (x >= VBE_mode_info->width || y >= VBE_mode_info->height){
		return;
	}
	
    Color* pixel = (Color*) getPixelPtr(x, y);

    pixel->r = color.r;
    pixel->g = color.g;
    pixel->b = color.b;
}

// inicio del bitmap
extern unsigned char _binary_font_psf_start[];

unsigned char *font = _binary_font_psf_start;

//coordenadas de donde escribir el proximo char
uint16_t current_X = 0;
uint16_t current_Y = 0;


void putChar(unsigned char c, int x, int y, Color fgcolor, Color bgcolor)
{
	int cx,cy;
	int mask[8]={1,2,4,8,16,32,64,128};
	unsigned char *glyph=font+4+(int)c*16;

	if (current_X >= VBE_mode_info->width) {
		current_X = 0;
        	if (current_Y + HEIGHT_FONT > VBE_mode_info->height) {
				current_Y -= HEIGHT_FONT;
            	scrollUp();
        	} else {
            		current_Y += HEIGHT_FONT;
        	}
    	}

	for(cy=0;cy<HEIGHT_FONT;cy++){
		for(cx=0;cx<WIDTH_FONT;cx++){
			putPixel(glyph[cy] & mask[cx] ? fgcolor : bgcolor, current_X + (8 - cx), current_Y + cy);
		}
	}	
	current_X += WIDTH_FONT;
	
}
void prints(const char *str, Color fnt, Color bgd){
    for (int i = 0 ; str[i] != '\0'; i++ ){
        print(str[i], fnt, bgd);
    }
}

void printHex(const uint64_t num, Color fnt, Color bgd) {
	char hexChars[] = "0123456789ABCDEF";
	char toPrint[SIZE];
	for(int i = SIZE - 1 ; i >= 0 ; i--) {
		toPrint[i] = hexChars[(num >> (i * 4)) & 0xF];	
	}
	prints(toPrint, fnt, bgd);
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

            putChar(c, current_X , current_Y , fnt , bgd);
        	break;
    }
}

void scrollUp (){
    Color* pixel, *next;
    for (int i = 0 ; i < current_Y + HEIGHT_FONT ; i++){
        for (int j = 0 ; j < VBE_mode_info->width ; j++){
            pixel = (Color *) getPixelPtr(j,i);
            next = (Color *) getPixelPtr(j,i+HEIGHT_FONT);
            *pixel = *next;
        }
    }
}

uint32_t* getPixelPtr(uint16_t x, uint16_t y) {
    uint8_t pixelwidth = VBE_mode_info->bpp/8;
    uint16_t pixelHeight = VBE_mode_info->pitch;  

    uintptr_t pixelPtr = (uintptr_t)(VBE_mode_info->framebuffer) + (x * pixelwidth) + (y * pixelHeight);
    return (uint32_t*)pixelPtr;
}


void print_newline(){
    current_X = 0;
    current_Y += HEIGHT_FONT;

    if (current_Y + HEIGHT_FONT > VBE_mode_info->height){
        current_Y -= HEIGHT_FONT;
        scrollUp();
    }
}

void print_backspace(Color fnt, Color bgd){
    if (current_X >= WIDTH_FONT){
	current_X -= WIDTH_FONT;
    } else {
        current_X = VBE_mode_info->width - WIDTH_FONT;
	current_Y -= HEIGHT_FONT;
    }
    putChar(' ', current_X, current_Y, fnt, bgd);
    current_X -= WIDTH_FONT;
}
