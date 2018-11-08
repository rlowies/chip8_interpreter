/* CPU.c
 * CHIP8 - CPU
 * Font set at memory location 0x50 == 80 and onwards
 * Program starts at 0x200 (PC)
 *
 * @author Ron Lowies
 *
 *
 * License: GNU General Public License (GPL) v2 
 * ( http://www.gnu.org/licenses/old-licenses/gpl-2.0.html )
 *
 * References
 * Copyright (C) 2011 Laurence Muller / www.multigesture.net 
 */
#include <stdio.h>
#include <stdlib.h>
#include "include/CPU.h"
#include <time.h>
#include <stdint.h>

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
 
 //2 bytes for current opcode
 unsigned short opcode; 
 //4k of memory
 const int memSize = 4096; 
 unsigned char memory[memSize]; 

 //General purpose registers v0 - vE
 unsigned char V[16]; 
 
 //Index register
 unsigned short I; 
 
 //Kew press
 bool isPressed = false;
 
 //Program counter
 unsigned short pc; 

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
 
 bool drawFlag;
 bool endCycle = false;

 /*
  * The cpu needs to know if the screen is being drawn at times
  */
 bool CPU::getDrawFlag() 
 {
	 return drawFlag;
 }
 /*
  * Sets the draw flag to false as needed
  */
 void CPU::setDrawFlagFalse() 
 {
	 drawFlag = false;
 }
 /*
  * Returns the pixels that will be used to determine which ones need to be tuned on in SDL2
  */
 unsigned char CPU::getGfx(int i) {
     return gfx[i];
}
 
 //Initializers
 CPU::CPU(){}
 CPU::~CPU(){}
 
 
 void CPU::initialize() 
 {
	 //init memory and registers
	 printf("%d\n", 1);
	 
	 pc = 0x200; //Start of program
	 opcode = 0; 
	 I = 0;
	 sp = 0;
	 
	 //display
	 for(int i = 0; i < 2048; ++i) {
		 gfx[i] = 0;
	 }
    
	 //stack
	 //registers v0=vF (vF is carry register)
	 for(int i = 0; i < stackSize; ++i) 
	 {
		stack[i] = 0;
		V[i] = 0;
	 }
	 
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
	 
	 drawFlag = true;
	 srand(time(NULL));
 }
 
 /*
  * Reads opcodes and performs operation
  */
 void CPU::emulateCycle()
 {
	 //Fetch
	 opcode = memory[pc] << 8 | memory[pc + 1];
     
     //Decode
	 switch(opcode & 0xF000) 
     {
          /*
		  * Two opcodes starting with 0x0
		  */
		 case 0x0000:
         {
			switch(opcode & 0x000F)
			{
				case 0x0000: // 0x00E0: Clears the screen
					//Execute
				  for(int i = 0; i < 2048; ++i) {
					gfx[i] = 0x0;
                  }
					drawFlag = true;
					pc += 2;
                break;
				
				case 0x000E: // 0x00EE: Returns from sub-routine
				  --sp;
				  pc = stack[sp];
				  pc+=2;
                break;
				
				default:
					printf("Unknown opcode [0x0000]: 0x%X\n", opcode);          
            }
        break;
         }
        /*
		 * Opcode 1NNN jumps to address NNN
		 * 
		 */
        case 0x1000:
            pc = opcode & 0x0FFF;
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
		 * Opcode 0x3XNN,
		 * Skips the next instruction if VX equals NN.
		 */
		 case 0x3000:
		   if(V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) 
		   {
			 pc += 4;
		   }
		   else 
           {
			 pc +=2;
		   }
		 break;
         
         /*
          * Opcode 0x4XNN
          * Skips the next instruction if VX doesn't equal NN. 
          */
          case 0x4000:
            if(V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) 
            {
                pc+=4;
            }
            else
            {
                pc+=2;
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
		  * Opcode 0x7XNN
		  */
		case 0x7000:
		  V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
		  pc += 2;
		break;
		  
        /*
        * Handles cases starting with 0x8
        */
    case 0x8000:
    {
        switch(opcode & 0x000F)
        {
        /*
         * Opcode 0x8XY0
         * Sets VX to the value of VY.
         */
         case 0x0000:
           V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
           pc+=2;
         break;
         
         /*
         * Opcode 0x8XY2
         * Sets VX to VX & VY
         */
         case 0x0002:
           V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
           pc+=2;
         break;
         
          /*
          * Opcode 8XY3
          * Sets VX to VX xor VY.
          */
          case 0x0003:
          V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
          pc += 2;
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
         * Opcode 0x8XY5
         * VY is subtracted from VX.
         * VF is set to 0 when there's a borrow, 
         * and 1 when there isn't.
         */
         case 0x0005:
         {
           if(V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
           {
             V[0xF] = 0; //borrow
           } 
           else 
           {
             V[0xF] = 1;
           }
           V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
           pc+=2;
         break;
         }
         /*
         * Opcode 8XY6
         * Shifts VY right by one and stores the result to VX 
         * (VY remains unchanged). 
         * VF is set to the value of the least significant bit of VY before the shift.[2]
         */
         case 0x0006:
         V[0xF] = V[(opcode & 0x00F0) >> 8] & 0x1;
         V[(opcode & 0x0F00) >> 8]  >>= 1;
         pc+=2;
         break;
         
        
         
         default:
                printf("Unknown opcode [0x8000]: 0x%X\n", opcode);          
        }
        break;
    }
    
        
        /*
         * Opcode 0x9XY0
         * Skips the next instruction if VX doesn't equal VY. 
         */
         case 0x9000:
         {
           if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]) 
           {
             pc += 4;
           }
           else
           {
             pc += 2;
           }
         break;
         }
 
        /*
		 * Opcode 0xANNN: Sets I to the address NNN
		 */
		 case 0xA000:
		   I = 0x0FFF & opcode;
		   pc += 2; //Each opcode is 2bytes long
		 break;
         
         /*
		 * Opcode 0xCXNN
		 * 
		 */
		case 0xC000:
		  V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);
		  pc+=2;
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
 
         drawFlag = true;
         pc += 2;
	    }
        break;
    
		case 0xE000:
        {
		  switch(opcode & 0x00FF)
		  {
		/*
		 * Opcode EX9E: Skips the next instruction 
		 * if the key stored in VX is pressed
		 */ 
		  case 0x009E:
            if(key[V[(opcode & 0x0F00) >> 8]] != 0) 
            {
			  pc += 4;
            }
		    else 
            {
			  pc += 2;
            }
          break;
          
          /*
           * Opcode 0xEXA1 
           * Skips the next instruction
           * if the key stored in VX isn't pressed. 
           */
           case 0x00A1:
             if(key[V[(opcode & 0x0F00) >> 8]] == 0) 
             {
               pc+=4;
             }
             else 
             {
               pc+=2;
             }
           break;
          
          default:
		   printf("Unknown opcode 0xE000: 0x%X\n", opcode);
		  }
		 break;
        }
    
         case 0xF000:
         {
		  switch(opcode & 0x00FF) 
		  {
              
            /*
             * Opcode 0xF00A
             * A key press is awaited, and then stored in VX.
             */
             case 0x000A:
             {
             
             for(int i = 0; i <= 15; i++) {
                 if(key[i] != 0) {
                 V[(opcode & 0x0F00) >> 8] = i;
                 isPressed = true;
                 }
                 
                 if(!isPressed) {
                     return; //re-cycle
                 }
             }
             pc+=2;
             }
             break;
            /*
			 * Opcode 0xFX07 
			 * Sets VX delay to value of delay timer.
			 */
			 case 0x0007:
			   V[(opcode & 0x0F00) >> 8] = delay_timer;
			   pc += 2;
			 break;
            /*
            * Opcode 0xFX15 
            * Sets timer delay to VX
            */
            case 0x0015:
              delay_timer = V[(opcode & 0x0F00) >> 8];
              pc += 2;
            break;
             
           /*
            * Opcode 0xFX18
            * Sets the sound timer to VX.
            */
             case 0x0018:
               sound_timer = V[(opcode & 0x0F00) >> 8];
               pc+=2;
             break;
             
           /*
            * Opcode 0xFX1E
            */
             case 0x001E:
             I += V[(opcode & 0x0F00) >> 8];
             pc+=2;
             break;
           /*
            * Opcode 0xFX29 
			* Set "I" to the location of sprite for character
			* in VX. 4x5 font
			*
			*/
			 case 0x0029:
			   I = V[(opcode & 0x0F00) >> 8] * 0x5;
			   pc+=2;
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
			
			/*
			 * Opcode 0xFX65
			 */
			 case 0x0065:
			 {
			 
			   for(int i = 0; i <= ((opcode & 0x0F00)>>8); ++i) 
               {
                 V[i] = memory[I + i];
			   }
			 
			   I += ((opcode & 0x0F00) >> 8) + 1;
			   pc+=2;
			 
			 break;
			 }
        
			 default:
					printf("Unknown opcode [0xF000]: 0x%X\n", opcode);          
		  }
		  break;
         }
		 
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
        }  
		--sound_timer;
	  }
      
 }
 /*
  * Loads file to CPU as binary
  */
 bool CPU::loadFile(char * fN)
 {
	 initialize();

	 FILE * pFile;
	 
	 pFile = fopen(fN, "rb");
	 puts(fN);
    
	 if(pFile != NULL)
	 {
		 
		 //Filesize
		 fseek(pFile,0,SEEK_END);
		 long size = ftell(pFile);
		 rewind(pFile); //Set pos to beginning
         char * buffer = (char*) malloc(sizeof(char) * size);
		 if(buffer == NULL)
		 {
			 fputs("Ram error", stderr);
             return false;
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
 
 
 