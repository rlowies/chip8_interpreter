CC = mingw32-g++.exe
INCLFLAGS = -IC:\Libraries\i686-w64-mingw32\include\SDL2
CXXFLAGS = $(INCLFLAGS) -std=c++0x -g -O3 -w -Wl,-subsystem,windows


TARGET = chip8

all: main.cpp
	$(CC) main.cpp CPU.cpp $(CXXFLAGS) -o $(TARGET) -lmingw32 -LC:/SDL/SDL2/i686-w64-mingw32/lib -lSDL2main -lSDL2
	
run: all
	./chip8.exe pong.rom
	
clean: 
	rm chip8.exe
	
	
