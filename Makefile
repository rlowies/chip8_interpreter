
all: main.cpp 
	g++ main.cpp CPU.cpp -o chip8
	
run: all
	./chip8
	
clean: 
	rm chip8
	
