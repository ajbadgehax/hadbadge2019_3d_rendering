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
#define STUPID_OFFH 16

#define MAP_WIDTH 16
#define MAP_HEIGHT 8

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
	for(int i=0; i<((FB_WIDTH+STUPID_OFFW) * FB_HEIGHT); i++){
		fbmem[i] = 0;						// Clear the framebuffer
	}
	cache_flush(fbmem, fbmem+FB_WIDTH*FB_HEIGHT);
	// GFX_REG(GFX_LAYEREN_REG)=GFX_LAYEREN_FB_8BIT|GFX_LAYEREN_FB|GFX_LAYEREN_TILEA;
	// GFX_REG(GFX_LAYEREN_REG)=GFX_LAYEREN_FB_8BIT|GFX_LAYEREN_FB|GFX_LAYEREN_TILEA;	// Emable framebuffer and layer A
	GFX_REG(GFX_LAYEREN_REG)=GFX_LAYEREN_FB_8BIT|GFX_LAYEREN_FB;	// Emable framebuffer
}
void setcolors(){
	for(int i=0; i<256; i++){
		GFXPAL[i] = COMP_COLOR(0xFF, i, i, i);
	}
	// GFXPAL[0] = COMP_COLOR(0xFF, 0x00, 0x00, 0x00);
	// GFXPAL[1] = COMP_COLOR(0xFF, 0xFF, 0xFF, 0xFF);
	// GFXPAL[0] = COMP_COLOR(0xFF, 0, 0, 0xFF);
}
const uint8_t b_m16_interleave[] = { 0, 49, 49, 41, 90, 27, 117, 10 };
uint8_t sin8_C( uint8_t theta){
    uint8_t offset = theta;
    if(theta & 0x40){
        offset = (uint8_t)255 - offset;
    }
    offset &= 0x3F; // 0..63
    uint8_t secoffset = offset & 0x0F; // 0..15
    if(theta & 0x40){
		secoffset++;
	}
    uint8_t section = offset >> 4; // 0..3
    uint8_t s2 = section * 2;
    const uint8_t* p = b_m16_interleave;
    p += s2;
    uint8_t b = *p;
    p++;
    uint8_t m16 = *p;
    uint8_t mx = (m16 * secoffset) >> 4;
    int8_t y = mx + b;
    if(theta & 0x80){
		y = -y;
	}
    y += 128;
    return y;
}
uint8_t cos8_C( uint8_t theta){
    return sin8_C(theta + 64);
}

void main(int argc, char **argv){
	setupscreen();
	setcolors();
	
	float playerX=2.0;
	float playerY=3.0;
	float playerA=0.0;
	
	float FOV=3.1415/2;
	uint8_t depth=0.0;
	if(MAP_WIDTH>MAP_HEIGHT){
		depth=MAP_WIDTH;
	}else{
		depth=MAP_HEIGHT;
	}
	float rayA;
	float distowal;
	uint8_t hitwall;
	float eyeX;
	float eyeY;
	int tmpX;
	int tmpY;
	int ceil;
	int floor;
	
	int gamemap[MAP_WIDTH][MAP_HEIGHT] = {
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1}
	};
	
	// Main Game Loop
	int run = 0;
	while(run == 0){
		
		if(MISC_REG(MISC_BTN_REG) & (BUTTON_LEFT)){
			playerA -= 0.1;
    	}
		if(MISC_REG(MISC_BTN_REG) & (BUTTON_RIGHT)){
			playerA += 0.1;
    	}
		
		if(MISC_REG(MISC_BTN_REG) & (BUTTON_UP)){
			playerX -= ((sin8_C(playerA*40+0x7F)-0x7F)/128.0) * 5;
			playerY -= ((cos8_C(playerA*40+0x7F)-0x7F)/128.0) * 5;
    	}
		if(MISC_REG(MISC_BTN_REG) & (BUTTON_DOWN)){
			playerX += ((sin8_C(playerA*40+0x7F)-0x7F)/128.0) * 5;
			playerY += ((cos8_C(playerA*40+0x7F)-0x7F)/128.0) * 5;
    	}

		
		
		for(int screenW=0; screenW<FB_WIDTH+STUPID_OFFW; screenW++){
			rayA=(playerA-FOV/2)+(screenW/(FB_WIDTH+STUPID_OFFW))*FOV;
			
			
			distowal=0.0;
			hitwall=0;
			
			eyeX=sin(rayA);
			eyeY=cos(rayA);

			// eyeX=(sin8_C(rayA*40+0x7F)-0x7F)/128.0;
			// eyeY=(cos8_C(rayA*40+0x7F)-0x7F)/128.0;
			
			while(hitwall!=1 && distowal<depth){
				distowal += 0.1;
				
				tmpX=(playerX+eyeX*distowal);
				tmpY=(playerY+eyeY*distowal);
				
				if(tmpX<0 || tmpX>=MAP_WIDTH || tmpY<0 || tmpY>=MAP_HEIGHT){
					hitwall=1;
					distowal=depth;
				}else{
					if(gamemap[tmpX][tmpY]==1){
						hitwall=1;
					}
				}
			}
			
			ceil = (FB_HEIGHT/2)-(FB_HEIGHT/distowal/2);
			floor = FB_HEIGHT - ceil;
			
			
			
			for(int screenH = 0; screenH < FB_HEIGHT+STUPID_OFFH; screenH++){
				
				if(screenH <= ceil){
					fbmem[(screenH*(FB_WIDTH+STUPID_OFFW))+screenW] = 0;
				}else if(screenH > ceil && screenH < floor){
					fbmem[(screenH*(FB_WIDTH+STUPID_OFFW))+screenW] = 255 - (distowal * 15);
				}else if(screenH >= floor){
					fbmem[(screenH*(FB_WIDTH+STUPID_OFFW))+screenW] = 0;
				}
			}
		}
		cache_flush(fbmem, fbmem+FB_WIDTH*FB_HEIGHT);
		if(MISC_REG(MISC_BTN_REG) & (BUTTON_START)){
			run = 1;
    	}
	}
	
		// for(int i = 0; i< 100; i++){
			// for(int h = 0; h<FB_HEIGHT+STUPID_OFFH; h++){
				// for(int w = 0; w<FB_WIDTH+STUPID_OFFW; w++){
					
					// if(i%2==0){
						// fbmem[h*(FB_WIDTH+STUPID_OFFW)+w] = 0;
					// }else{
						// fbmem[h*(FB_WIDTH+STUPID_OFFW)+w] = 255;
					// }


					
					
				// }
			// }
		// }
		
		// for(int h = 0; h<FB_HEIGHT+STUPID_OFFH; h++){
			// fbmem[h*(FB_WIDTH+STUPID_OFFW)+(FB_WIDTH/2)] = 1;
		// }
		
		// for(int w = 0; w<FB_WIDTH+STUPID_OFFW; w++){
			// fbmem[w+(FB_WIDTH*10)] = 1;
		// }
		
		
		// cache_flush(fbmem, fbmem+FB_WIDTH*FB_HEIGHT);
	// while(run == 0){
		// if(MISC_REG(MISC_BTN_REG) & (BUTTON_START)){
			// run = 1;
		// }
	// }
	
	
}



// (screenH*FB_WIDTH)+screenW
// (screenW*FB_HEIGHT)+screenH