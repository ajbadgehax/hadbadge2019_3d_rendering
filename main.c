#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "mach_defines.h"
#include "sdk.h"
#include "gfx_load.h"
#include "cache.h"

//Pointer to the framebuffer memory.
uint8_t *fbmem;

#define FB_WIDTH 512
#define FB_HEIGHT 320

#define COMP_COLOR(A, R, G, B) ((((A) & 0xFF) << 24) | \
								(((B) & 0xFF) << 16) | \
								(((G) & 0xFF) <<  8) | \
								(((R) & 0xFF) <<  0))
// #define FB_PIX(X, Y) fbmem[(X) + ((Y) * FB_WIDTH)]

void main(int argc, char **argv){
	GFX_REG(GFX_BGNDCOL_REG)=0;				// Bleck
	GFX_REG(GFX_LAYEREN_REG)=0;				// Disable all gfx layers
	fbmem=calloc(FB_WIDTH,FB_HEIGHT);		// Allocate screen buffer

	GFX_REG(GFX_FBPITCH_REG)=(GFX_FBPITCH_PAL_OFF)|(FB_WIDTH<<GFX_FBPITCH_PITCH_OFF);	// Declare Pitch for the color Palette
	GFX_REG(GFX_FBADDR_REG)=((uint32_t)fbmem);

	// Clear the framebuffer
	for(int i=0; i<(FB_WIDTH * FB_HEIGHT); i++){
		fbmem[i] = 0;
	}

	// GFX_REG(GFX_LAYEREN_REG)=GFX_LAYEREN_FB_8BIT|GFX_LAYEREN_FB|GFX_LAYEREN_TILEA;	// Emable framebuffer
	GFX_REG(GFX_LAYEREN_REG)=GFX_LAYEREN_FB;	// Emable framebuffer

	// Set colors
	GFXPAL[0] = COMP_COLOR(0xFF, 0xFF, 0xFF, 0xFF);
	GFXPAL[1] = COMP_COLOR(0xFF, 0x00, 0x00, 0xFF);
	GFXPAL[2] = COMP_COLOR(0xFF, 0x00, 0xFF, 0x00);
	GFXPAL[3] = COMP_COLOR(0xFF, 0xFF, 0x00, 0x00);
	
	// Set pixels to the color
	for(int i=0; i<(FB_WIDTH * FB_HEIGHT); i++){
		switch(i%4){
			case 0:
				fbmem[i] = 0;
				break;
			case 1:
				fbmem[i] = 1;
				break;
			case 2:
				fbmem[i] = 2;
				break;
			case 3:
				fbmem[i] = 3;
				break;
		}
	}
	
	// Update screen
	cache_flush(fbmem, fbmem+FB_WIDTH*FB_HEIGHT);
	
	
	int run = 1;
	while(run){
		if(MISC_REG(MISC_BTN_REG) & (BUTTON_START)){
			run = 0;
    	}
	}
}
