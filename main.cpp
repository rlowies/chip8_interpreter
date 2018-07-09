//CHIP8 INTERPRETER
#include "include/CPU.h"
#include "library/SDL2/SDL.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


CPU cpu; 
char fileName[255];

const int pixel_w = 64;
const int pixel_h = 32;

SDL_Renderer* renderer;
SDL_Texture* texture;
SDL_Window* window;
SDL_Surface* screenSurface;

uint32_t* pixels;


void setupGraphics() 
{
	//window size
	window = NULL;
	//display mode
	screenSurface = NULL;
    
	
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL Error: %s,\n", SDL_GetError());
	} 
	else
	{
        
		 //Create window
        window = SDL_CreateWindow( "Chip8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
		else 
		{
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        128,
        64);
        pixels = new uint32_t[ 2048 ];
        
        
//        screenSurface = SDL_CreateRGBSurfaceFrom(NULL,640,480,32,0,
//        0x00FF0000,
//        0x0000FF00,
//        0x000000FF,
//        0xFF000000);
//        
       
		
		
		}
	}
	
	
}

void setupInput() 
{
  
	//input system (bind callbacks)
}

void drawGraphics() {
    int pitch;
     SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
     for(int i = 0; i < 2048; i++) {
         if(!cpu.getGfx(i)) {
             pixels[i] = 0x000000;
         }
         else {
             pixels[i] = 0xFFFFFF;
         }
     }
     SDL_UnlockTexture(texture);
    
     SDL_RenderClear(renderer);
     SDL_RenderCopy(renderer,texture,NULL,NULL);
     SDL_RenderPresent(renderer);
    
	cpu.setDrawFlagFalse();
    
}

int main(int argc, char **argv)
 {

	
	if(argc == 2) 
	{
		strcat(fileName, "games/");
		strcat(fileName, argv[1]);
		printf("%s\n", fileName);
	} else 
	{
		printf("Usage: chip8 <fileName>");
	}
	
	setupGraphics();
	setupInput();
	
	
	
	//Initialize cpu and load game into memory
	cpu.initialize();
	cpu.loadFile(fileName);
	
	
	
	//Interpretation loop 
	while(1)
	{
		  
		cpu.emulateCycle();
		
		//if drawflag set 
		if(cpu.getDrawFlag()) {
			drawGraphics();
		SDL_UpdateWindowSurface(window);
		
		
        }
       
		
		SDL_Delay(1);
		// Store key press state (User input)
		//cpu.setKeys();
		
		
	}
    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
	
	return 0;
}

