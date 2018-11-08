/*
 * CHIP8 INTERPRETER
 * 
 * Built as a proof of concept
 *
 *@author Ron Lowies
 */
#include "include\CPU.h"
#include "i686-w64-mingw32\include\SDL2\SDL.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//CPU variable
CPU cpu; 

//Max filename size is 256
char fileName[255];

//Size of a pixel
const int pixel_w = 64;
const int pixel_h = 32;

//Grapics variables
SDL_Renderer* renderer;
SDL_Texture* texture;
SDL_Window* window;
SDL_Surface* screenSurface;
uint32_t* pixels;
const Uint8* state;

//Chip8 key set is 0x0 - 0xF
char keys[] = {
    SDL_SCANCODE_1,
    SDL_SCANCODE_2,
    SDL_SCANCODE_3,
    SDL_SCANCODE_4,
    SDL_SCANCODE_Q,
    SDL_SCANCODE_W,
    SDL_SCANCODE_E,
    SDL_SCANCODE_R,
    SDL_SCANCODE_A,
    SDL_SCANCODE_S,
    SDL_SCANCODE_D,
    SDL_SCANCODE_F,
    SDL_SCANCODE_Z,
    SDL_SCANCODE_X,
    SDL_SCANCODE_C,
    SDL_SCANCODE_V
};

/*
 * Checks the state of keyboard input
 */
void getKeys() 
{
    SDL_PumpEvents();
    
    state = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
    for(int i = 0; i <= 15; i++) {
		//If the the key is pressed send it to the CPU
        cpu.key[i] = state[keys[i]] == 1;
    }
}

void setupGraphics() 
{
	window = NULL;
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
		//Standard window creation procedure
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,
		//Pixel sizes
        SDL_TEXTUREACCESS_STREAMING, pixel_w, pixel_h);
        pixels = new uint32_t[ pixel_w * pixel_h ];
		}
	}
	
	
}

void drawGraphics() {
    int pitch;
	//Similar to a mutex
     SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch);
     for(int i = 0; i < 2048; i++) {
		 //If the pixel is on set it to white
         if(!cpu.getGfx(i)) {
             pixels[i] = 0x00000000; // White
         }
         else {
             pixels[i] = 0xFFFFFFFF; // Black
         }
     }
     SDL_UnlockTexture(texture);
    
	//Clean and render changes
     SDL_RenderClear(renderer);
     SDL_RenderCopy(renderer,texture,NULL,NULL);
     SDL_RenderPresent(renderer);
    
	//Let the cpu know we are done drawing
    cpu.setDrawFlagFalse();
    
}

int main(int argc, char **argv)
 {
	//Needed to allow SDL_Quit to stop the loop
	int running = 1;
	
	//Check arguments
	if(argc == 2) 
	{
		strcat(fileName, "games\\");
		strcat(fileName, argv[1]);
		printf("%s\n", fileName);
	} else 
	{
		printf("Usage: chip8 <fileName>");
	}
	
	//Initialize graphics
	setupGraphics();
	
	//Initialize cpu and load game into memory
	cpu.initialize();
	cpu.loadFile(fileName);
	
	
	
	//Interpretation loop 
	while(running)
	{
        SDL_Event event;
		cpu.emulateCycle();
		
		//if drawflag set 
		if(cpu.getDrawFlag()) {
			drawGraphics();
        }
         
		//Not optimal but enough to work.
		SDL_Delay(1);
		
		// Store key press state (User input)
        if(SDL_PollEvent(&event)); {
		getKeys();
        
		if(event.type == SDL_QUIT) {
		running = 0;
		}
        }
	}
	//Clean up
    delete[] pixels;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}

