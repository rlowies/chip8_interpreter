//CHIP8 INTERPRETER
#include "include/CPU.h"
#include "library/SDL2/SDL.h"

#include <stdio.h>
#include <string.h>
CPU cpu; 
char fileName[255];

void setupGraphics() 
{
	//window size
	SDL_Window* window = NULL;
	//display mode
	SDL_Surface* screenSurface = NULL;
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL Error: %s,\n", SDL_GetError());
	} 
	else
	{
		 //Create window
        window = SDL_CreateWindow( "Chip8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
		else 
		{
		screenSurface = SDL_GetWindowSurface(window);
		
		SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
		
		SDL_UpdateWindowSurface(window);
		
		SDL_Delay(2000);
		}
	}
	
	
}

void setupInput() 
{
	//input system (bind callbacks)
}

void drawGraphics() {
	
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
	for(;;) 
	{
		  
		cpu.emulateCycle();
		
		//if drawflag set 
		if(cpu.getDrawFlag()) {
			drawGraphics();
		}
		
		
		// Store key press state (User input)
		//cpu.setKeys();
		
		
	}
	
	return 0;
}

