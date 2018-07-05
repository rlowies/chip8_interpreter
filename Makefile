

all: main.cpp 
	g++ main.cpp CPU.cpp -o chip8
	
run: all
	./chip8 pong.rom
	
clean: 
	rm chip8
	
