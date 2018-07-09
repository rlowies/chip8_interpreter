//CPU Header
#include <stdint.h>


class CPU 
{
public:
    CPU();
    ~CPU();
    unsigned char getGfx(int i);
	void initialize(); //Init registers
	void emulateCycle(); //Fetch, Decode, Execute. Update timers.
	bool loadFile(char * fN); //Load the file in binary format
	bool getDrawFlag(); 
    void setDrawFlagFalse(); 
    bool endCycle;

 
};




