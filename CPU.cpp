//CHIP8 - CPU
//Font set at memory location 0x50 == 80 and onwards
//Program starts at 0x200 (PC)

#include <stdio.h>
#include <stdlib.h>
#include "include/CPU.h"

 unsigned char chip8_fontset[80] =
 { 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
 };

 unsigned short opcode; //2 bytes for current opcode
 const int memSize = 4096;
 unsigned char memory[memSize]; //4k of memory

 unsigned char V[16]; //General purpose registers v0 - vE

 unsigned short I; //Index register

 unsigned short pc; //Program counter

 /* MEMORY MAP
  * 
  * 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
  * 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
  * 0x200-0xFFF - Program ROM and work RAM
  */
 
 //Drawing is done in XOR mode
 //Pixel state is 1 or 0
 unsigned char gfx[64 * 32]; //2048 pixels
 
 //60hz timer registers
 //buzzer when timer is 0
 unsigned char delay_timer;
 unsigned char sound_timer;
 
 //Stack and stack pointer
 const int stackSize = 16;
 unsigned short stack[stackSize];
 unsigned short sp;
 
 //Keypad (0x0-0xF)
 unsigned char key[16];
 
 
 void CPU::initialize() 
 {
	 //init memory and registers
	 printf("%d\n", 1);
	 
	 pc = 0x200; //Start of program
	 opcode = 0; 
	 I = 0;
	 sp = 0;
	 
	 //display
	 //stack
	 for(int i = 0; i < stackSize; ++i) {
		 stack[i] = 0;
	 }
	 //registers v0=vF (vF is carry register)
	 //memory
	 for(int i = 0; i < memSize; ++i) 
	 {
		 memory[i] = 0;
	 }
	 
	 //Load font
	 for(int i = 0; i < 80; ++i) 
	 {
		 memory[i] = chip8_fontset[i];
	 }
	 
	 //Timers
	 delay_timer = 0;
	 sound_timer = 0;
	
 }
 
 void CPU::emulateCycle()
 {
	 //Fetch
	 opcode = memory[pc] << 8 | memory[pc + 1];
	 
     //Decode
	 switch(opcode & 0xF000) {
		/*
		 * Opcode 0xANNN: Sets I to the address NNN
		 */
		 case 0xA000:
		 I = 0x0FFF & opcode;
		 pc += 2; //Each opcode is 2bytes long
		 break;
		 
		/*
		 * Opcode 0x2NNN Calls subroutine at address NNN
		 */
		 case 0x2000:
			stack[sp] = pc;
			++sp;
			pc = opcode & 0x0FFF;
		break;
		/*
		 * Opcode 0x8XY4
		 */
		case 0x0004: 
		if(V[(opcode & 0x00F0) >> 4] > (0xFF - V[(opcode & 0x0F00) >> 8]))
		{
		  V[0xF] = 1; //carry
		}
		else
		{
		  V[0xF] = 0;
		}
		V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
		pc += 2;          
		break;
		 /*
		  * Two opcodes starting with 0x0
		  */
		 case 0x0000:
			switch(opcode & 0x000F)
			{
				case 0x0000: // 0x00E0: Clears the screen
					//Execute
				break;
				
				case 0x000E: // 0x00EE: Returns from sub-routine
					// Execute opcode
				break;
				
				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", opcode);          
			}
		break;
		
		 case 0xF000:
		  switch(opcode & 0x00FF) 
		  {
			  
		   /*
			* Opcode 0xFX33
			* Binary encoded decimal representations of VX at address I. + 1, + 2
			*/
			//TJA's Solution
			case 0x0033:
		
			memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
			memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
			memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
			pc += 2;
		
			break;
			
			/*
			 * Opcode 0xFX65
			 */
			 case 0x0065:
			 {
			 unsigned int X = opcode & 0x0F00;
			 for(int i = 0; i <= X; ++i) {
				memory[I] = V[i];
				I += 1;
			 }
			 pc+=2;
			 
			 break;
			 
			 /*
			  * Opcode 0xFX29 
			  * Set "I" to the location of sprite for character
			  * in VX. 4x5 font
			  *
			  */
			  case 0x0029:
			  //TODO:
			  break;
			 }
			
			
			default:
					printf("Unknown opcode [0x0000]: 0x%X\n", opcode);          
		  }
		  break;
		/*
		 * Opcode 0x6XNN
		 */
		case 0x6000:
		V[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
		pc += 2;
		break;
		
		/*
		 * Opcode 0xDXYN
		 */
		 case 0xD000:	
		 {

        unsigned short x = V[(opcode & 0x0F00) >> 8];
		unsigned short y = V[(opcode & 0x00F0) >> 4];
		unsigned short height = opcode & 0x000F;
		unsigned short pixel;
 
		V[0xF] = 0;
		for (int yline = 0; yline < height; yline++)
		{
			pixel = memory[I + yline];
			for(int xline = 0; xline < 8; xline++)
			{
				if((pixel & (0x80 >> xline)) != 0)
			{
			if(gfx[(x + xline + ((y + yline) * 64))] == 1)
			V[0xF] = 1;                                 
			gfx[x + xline + ((y + yline) * 64)] ^= 1;
			}
			}
       }
 
         //drawFlag = true;
         pc += 2;
	 }
         break;
		 
		 default:
		   printf("Unknown opcode: 0x%X\n", opcode);
	 }
	 //Update Timers
	  if(delay_timer > 0) {
		--delay_timer;
	  }
	  if(sound_timer > 0)
      {
        if(sound_timer == 1)
		{
         printf("BEEP!\n");
         --sound_timer;
        }  
	  }
 }
 //In progress
 bool CPU::loadFile(char * fN)
 {
	 initialize();
	 
	 char * buffer;
	 
	 FILE * pFile;
	 
	 pFile = fopen(fN, "rb");
	 puts(fN);
	 
	  
	 
	 if(pFile != NULL)
	 {
		// setvbuf(pFile, buffer, _IOFBF, 4096);
		 
		 //Filesize
		 fseek(pFile,0,SEEK_END);
		 long size = ftell(pFile);
		 rewind(pFile); //Set pos to beginning
		 buffer = (char*) malloc(sizeof(char) * size);
		 if(buffer == NULL)
		 {
			 fputs("Ram error", stderr);
		 }
		 
		 size_t res = fread(buffer, 1, size, pFile);
		 
		 if(res != size) 
		 {
			 fputs("Error reading", stderr);
			 return false;
		 }
		 
		 if((memSize-512) > size)
		 {
		 for(int i = 0; i < size; ++i) 
		 {
		 memory[i + 512] = buffer[i];
		 }
		 } 
		 else
		 {
			 printf("Rom too large!");
		 }
		
		
		 
		 fclose(pFile);
		 free(buffer);
		 
		 return true;
	 }
	 
	
	 
	 
 }
 
 
 