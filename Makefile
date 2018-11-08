CC = mingw32-g++.exe
INCLFLAGS = -IC:\Users\ronlo\Documents\git\chip8_cpp\i686-w64-mingw32\include\SDL2
LIBPATHS = C:\Users\ronlo\Documents\git\chip8_cpp\i686-w64-mingw32\lib
CXXFLAGS = $(INCLFLAGS) -std=c++0x -g -O3 -w -Wl,-subsystem,windows


TARGET = chip8

all: main.cpp
	$(CC) main.cpp CPU.cpp $(CXXFLAGS) -o $(TARGET) -lmingw32 -L$(LIBPATHS) -lSDL2main -lSDL2
	
run: all
	./chip8.exe tetris.c8
	
clean: 
	del /f chip8.exe
	
	
