# tested with: g++-11
CC 		= g++
CFLAGS  = -g -Wall 
SRC_DIR 	= src
BUILD_DIR 	= build
TARGET 		= minesweeper

SRC = ${wildcard $(SRC_DIR)/*.cpp}
OBJ = ${patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,${SRC}}

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	mkdir -p ${dir $@}
	$(CC) $(CFLAGS) -o $@ $< -c

all: ${OBJ}
	$(CC) $(CFLAGS) -I$(SRC_DIR) -o $(TARGET) ${OBJ}

opd:
	echo $(OBJ)
	$(CC) $(CFLAGS) -o po.o opdracht/pointer_opdracht.cc 
	$(CC) $(CFLAGS) -o od.o opdracht/opdracht_dubbel.cc 

clean:
	$(RM) -rf $(BUILD_DIR)
