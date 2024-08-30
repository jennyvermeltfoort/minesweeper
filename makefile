# tested with: g++-11
CC 		= g++
CFLAGS  = -g -Wall 
SRC 	= src
TARGET 	= $(SRC)/main.cpp $(SRC)/prompt.cpp $(SRC)/tim.cpp

main: $(TARGET)
	$(CC) $(CFLAGS) -o main $(TARGET)

all: main

clean:
	$(RM) $(TARGET)