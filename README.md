# Chip8 Interpreter In C++

The chip8 is a processor originally used in the Cosmac VIP and Telmac 1800 microcomputers in the 1970's.
I chose to write this interpreter as a challenge to myself and out of pure interest in emulation. 

### Compiling on Windows

First you will need the [SDL2 development libraries](https://www.libsdl.org/download-2.0.php). be sure to select the correct version from the zip file (32-bit) 

The makefile will look for the header file and libraries under

```
C:\Users\username\PathToGit\chip8_cpp\i686-w64-mingw32\include\SDL2
C:\Users\username\PathToGit\chip8_cpp\i686-w64-mingw32\include\lib
```

Be sure to edit the makefile to your paths.

You will need to use [MinGW](https://sourceforge.net/projects/mingw/files/) to compile.
During installation check the mingw32-base, mingw32-gcc-g++, and msys-base check boxes.

This will take a few minutes, once complete you will need to add C:\MinGW\bin at the top of the System Environment PATH variable.
Restart CMD and simply type

```
$ make 
$ make run
```

For now there is no way to load other games once the interpreter has launched, you can change which game is loaded within the makefile.

### Bugs

Some games run great but there are a few issues, and at this time not all opcodes are programmed. However the cpu is operational as a proof of concept.

### Conclusion

This was the first project I wrote under the C++ language. I have experience in C and that helped a lot, a microprocessors course aided my understanding of opcode operations and a Programming Languages course was the foundation to my interpretation knowledge.
