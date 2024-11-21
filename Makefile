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

plot: ${OBJ}
	./${TARGET} -w 20 -h 20 -b 200 -r output.txt -i 10000
	gnuplot -c gnuplot.script

clean:
	$(RM) -rf $(BUILD_DIR)
