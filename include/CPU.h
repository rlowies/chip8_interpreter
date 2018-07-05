//CPU Header

class CPU 
{
  public:
	void initialize(); //Init registers
	void emulateCycle(); //Fetch, Decode, Execute. Update timers.
	bool loadFile(char * fN); //Load the file in binary format
	bool getDrawFlag(); 
};




