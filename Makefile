# tested with: g++-11
CC 		= g++
CFLAGS  = -g -Wall 
SRC_DIR 	= src
BUILD_DIR 	= build
TARGET 		= minesweeper

SRC = ${wildcard $(SRC_DIR)/*.cc}
OBJ = ${patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,${SRC}}

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cc
	mkdir -p ${dir $@}
	$(CC) $(CFLAGS) -o $@ $< -c

all: ${OBJ}
	$(CC) $(CFLAGS) -I$(SRC_DIR) -o $(TARGET) ${OBJ}

opd:
	echo $(OBJ)
	$(CC) $(CFLAGS) -o po.o opdracht/pointer_opdracht.cc 
	$(CC) $(CFLAGS) -o od.o opdracht/opdracht_dubbel.cc 

clean:
	$(RM) $(BUILD_DIR)