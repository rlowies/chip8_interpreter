
all: main.cpp 
	g++ main.cpp CPU.cpp -o chip8
	
run: chip8
	./chip8
	
clean: 
	rm chip8
	
