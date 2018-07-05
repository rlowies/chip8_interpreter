//CHIP8 INTERPRETER
#include "include/CPU.h"
#include <stdio.h>
#include <string.h>
CPU cpu; 
char fileName[20];

void setupGraphics() 
{
	//window size
	//display mode
	
	
}

void setupInput() 
{
	//input system (bind callbacks)
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
		/*
		//if drawflag set 
		if(cpu.drawFlag) {
			drawGraphics();
		}
		*/
		
		// Store key press state (User input)
		//cpu.setKeys();
		
		
	}
	
	return 0;
}

