# tested with: g++-11
CC 		= g++
CFLAGS  = -g -Wall 
SRC 	= src
TARGET 	= $(SRC)/main.cc 

main: $(TARGET)
	$(CC) $(CFLAGS) -o main.o $(TARGET)

opd: 
	$(CC) $(CFLAGS) -o po.o opdracht/pointer_opdracht.cc 
	$(CC) $(CFLAGS) -o od.o opdracht/opdracht_dubbel.cc 

all: main

clean:
	$(RM) $(TARGET)