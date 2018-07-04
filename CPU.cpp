//CHIP8 - CPU
//Font set at memory location 0x50 == 80 and onwards
//Program starts at 0x200 (PC)

#include <stdio.h>
#include "CPU.h"

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
	 
	 //Case where ANNN
	 if(0xF000 & opcode == 0xA000) {
		 //Execute
		 I = 0x0FFF & opcode;
	 }
	 
	 
	 pc += 2; //Each opcode is 2bytes long
	 
	 
	 
	 //Update Timers
 }
 //In progress
 void CPU::loadFile()
 {
	 bufferSize = 4;
	 char buffer[bufferSize];
	 
	 FILE * pFile;
	 
	 pFile = fopen("/file1.file", rb);
	 
	 if(pFile != NULL)
	 {
		 setvbuf(pFile, buffer, _IOFBF, bufferSize);
		 
		 for(int i = 0; i < bufferSize; ++i) {
		 memory[i+0x50] = buffer[i];
		}
		 
		 fClose(pFile);
	 }
	 
	 
	 
 }
 
 
 