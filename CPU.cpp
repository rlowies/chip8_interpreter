//CHIP8 - CPU
//Font set at memory location 0x50 == 80 and onwards
//Program starts at 0x200 (PC)

#include <stdio.h>
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

 unsigned char memory[4096]; //4k of memory

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
 unsigned short stack[16];
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
	 //registers v0=vF (vF is carry register)
	 
	 //Load font
	 for(int i = 80; i < 160; ++i) {
		 memory[i] = chip8_fontset[i - 80];
	 }
	 
	 //Timers
	
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
 void CPU::loadFile(char * fN)
 {
	 int bufferSize = 4;
	 char buffer[bufferSize];
	 
	 FILE * pFile;
	 
	 pFile = fopen(fN, "rb");
	 puts(fN);
	 
	 if(pFile != NULL)
	 {
		 setvbuf(pFile, NULL, _IOFBF, bufferSize);
		 
		 for(int i = 0; i < bufferSize; ++i) {
		 memory[i + 512] = buffer[i];
		}
		 
		 fclose(pFile);
	 }
	 
	 
	 
 }
 
 
 