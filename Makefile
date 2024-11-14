# tested with: g++-11
CC 		= g++
CFLAGS  = -g -Wall 
SRC 	= src
TARGETS	= $(SRC)/main.cc $(SRC)/world.cc $(SRC)/stack.cc

main: $(TARGETS)
	$(CC) $(CFLAGS) -I$(SRC) -o main.o $(TARGETS)  

opd: 
	$(CC) $(CFLAGS) -o po.o opdracht/pointer_opdracht.cc 
	$(CC) $(CFLAGS) -o od.o opdracht/opdracht_dubbel.cc 

all: main

clean:
	$(RM) $(TARGETS)