CC=g++
FILEEXT=.cpp
GENFLGS=-Wall -Wextra
LNKFLGS=-lOpenGL -lglfw -lGLEW -lGLU -lGL -lEGL
EXENAME=run

make: build

build:
	$(CC) -o ../bin/$(EXENAME) *$(FILEEXT) $(GENFLGS) $(LNKFLGS)
	./../bin/$(EXENAME) 
	read -p "Press Enter to Continue"
	clear

debug:
	$(CC) -g -o ../bin/$(EXENAME) *$(FILEEXT) $(GENFLGS) $(LNKFLGS)
	gdb ../bin/$(EXENAME)
	clear
