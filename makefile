CC=g++
FILEEXT=.cpp
EXENAME=run
GENFLGS=-Wall -Wextra -Werror
LNKFLGS=-lOpenGL -lglfw -lGLEW -lGL -lEGL -lGLU

make: build

build:
	$(CC) -o ../bin/$(EXENAME) *$(FILEEXT) $(GENFLGS) $(LNKFLGS)
	./../bin/$(EXENAME)
	read -p "Press Enter to Continue"
	clear

debug:
	$(CC) -g -o ../bin/$(EXENAME) *$(FILEEXT) $(GENFLGS) $(LNKFLGS)
	gdb ../bin/$(EXENAME)
	read -p "Press Enter to Continue"
	clear

release:
	$(CC) -g -o ../bin/$(EXENAME) *$(FILEEXT) $(GENFLGS) $(LNKFLGS)
	./..bin/$(EXENAME) 
	read -p "Press Enter to Continue"
	clear
