//CPU Header

class CPU 
{
  public:
	void initialize(); //Init registers
	void emulateCycle(); //Fetch, Decode, Execute. Update timers.
	void loadFile(char * fN); //Load the file in binary format
};




