#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "mach_defines.h"
#include "sdk.h"
#include "gfx_load.h"
#include "cache.h"

//Pointer to the framebuffer memory.
uint8_t *fbmem;

#define FB_WIDTH 480
#define FB_HEIGHT 320

#define STUPID_OFFW 16

#define MAP_WIDTH 16
#define MAP_HEIGHT 16

#define COMP_COLOR(A, R, G, B) ((((A) & 0xFF) << 24) | \
								(((B) & 0xFF) << 16) | \
								(((G) & 0xFF) <<  8) | \
								(((R) & 0xFF) <<  0))

void setupscreen(){
	GFX_REG(GFX_BGNDCOL_REG)=0;				// set bg to Bleck
	GFX_REG(GFX_LAYEREN_REG)=0;				// Disable all gfx layers
	fbmem=calloc(FB_WIDTH,FB_HEIGHT);		// Allocate screen buffer
	GFX_REG(GFX_FBPITCH_REG)=(GFX_FBPITCH_PAL_OFF)|(FB_WIDTH<<GFX_FBPITCH_PITCH_OFF);	// Declare Pitch for the color Palette
	GFX_REG(GFX_FBADDR_REG)=((uint32_t)fbmem);
	for(int i=0; i<(FB_WIDTH * FB_HEIGHT); i++){
		fbmem[i] = 0;						// Clear the framebuffer
	}
	// GFX_REG(GFX_LAYEREN_REG)=GFX_LAYEREN_FB_8BIT|GFX_LAYEREN_FB|GFX_LAYEREN_TILEA;
	// GFX_REG(GFX_LAYEREN_REG)=GFX_LAYEREN_FB_8BIT|GFX_LAYEREN_FB|GFX_LAYEREN_TILEA;	// Emable framebuffer and layer A
	GFX_REG(GFX_LAYEREN_REG)=GFX_LAYEREN_FB_8BIT|GFX_LAYEREN_FB;	// Emable framebuffer
}
void setcolors(){
	GFXPAL[0] = COMP_COLOR(0xFF, 0x00, 0x00, 0x00);
	GFXPAL[1] = COMP_COLOR(0xFF, 0xFF, 0xFF, 0xFF);
	GFXPAL[2] = COMP_COLOR(0xFF, 0, 0, 0xFF);
}

void main(int argc, char **argv){
	setupscreen();
	setcolors();
	
	// float playerX=2.0;
	// float playerY=2.0;
	// float playerA=5.0;
	
	// float FOV=3.1415/4;
	// float depth=0.0;
	// if(MAP_WIDTH>MAP_HEIGHT){
		// depth=MAP_WIDTH;
	// }else{
		// depth=MAP_HEIGHT;
	// }
	// float rayA;
	// float distowal;
	// uint8_t hitwall;
	// float eyeX;
	// float eyeY;
	// int tmpX;
	// int tmpY;
	// int ceil;
	// int floor;
	
	// int gamemap[MAP_WIDTH][MAP_HEIGHT] = {
		// {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		// {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
	// };
	
	// Main Game Loop
	int run = 0;
	// while(run == 0){
		// for(int screenW=0; screenW<FB_WIDTH; screenW++){
			// rayA=(playerA-FOV/2)+(screenW/FB_WIDTH)*FOV;
			
			// distowal=0.0;
			// hitwall=0;
			
			// eyeX=sin(rayA);
			// eyeY=cos(rayA);
			
			// while(hitwall!=1 && distowal<depth){
				// distowal += 0.5;
				
				// tmpX=(playerX+eyeX*distowal);
				// tmpY=(playerY+eyeY*distowal);
				
				// if(tmpX<0 || tmpX>=MAP_WIDTH || tmpY<0 || tmpY>=MAP_HEIGHT){
					// hitwall=1;
					// distowal=depth;
				// }else{
					// if(gamemap[tmpX][tmpY]==1){
						// hitwall=1;
					// }
				// }
			// }
			
			// ceil = (MAP_HEIGHT/2)-MAP_HEIGHT/distowal;
			// floor = MAP_HEIGHT-ceil;
			
			// for(int screenH = 0; screenH < FB_HEIGHT; screenH++){
				// if(screenH<ceil){
					// fbmem[(screenH*FB_WIDTH)+screenW] = 0;
				// }else if(screenH>ceil && screenH<=floor){
					// fbmem[(screenH*FB_WIDTH)+screenW] = 1;
				// }else{
					// fbmem[(screenH*FB_WIDTH)+screenW] = 0;
				// }
				
				
				
			// }
		// }
		// cache_flush(fbmem, fbmem+FB_WIDTH*FB_HEIGHT);	// Update screen
		// if(MISC_REG(MISC_BTN_REG) & (BUTTON_START)){
			// run = 1;
    	// }
	// }
	
	
		// for(int h = 0; h<FB_HEIGHT/2; h++){
			// for(int w = 0; w<FB_WIDTH/2; w++){
				
				

				// fbmem[h*(FB_WIDTH+16)+w] = 1;

				
				
			// }
		// }
		// for(int i=0; i<((FB_WIDTH+STUPID_OFFW) * FB_HEIGHT); i++){
			// fbmem[i] = 0;						// Clear the framebuffer
		// }
		
		for(int h = 0; h<FB_HEIGHT+16; h++){
			fbmem[h*(FB_WIDTH+STUPID_OFFW)+50] = 1;
		}
		
		
		cache_flush(fbmem, fbmem+FB_WIDTH*FB_HEIGHT);
	while(run == 0){
		if(MISC_REG(MISC_BTN_REG) & (BUTTON_START)){
			run = 1;
		}
	}
	
	
}



// (screenH*FB_WIDTH)+screenW
// (screenW*FB_HEIGHT)+screenH