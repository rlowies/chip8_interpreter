//CHIP8 - CPU

#include <stdio.h>
#include "CPU.h"

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
 }
 
 void CPU::emulateCycle()
 {
	 //Fetch
	 //Decode
	 //Execute
	 
	 //Update Timers
 }
 
 
 