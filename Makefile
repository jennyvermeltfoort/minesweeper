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

plot: all
	./${TARGET} -w 20 -h 20 -b 50 -r output_20_20_50.txt -i 100000
	./${TARGET} -w 20 -h 20 -b 20 -r output_20_20_20.txt -i 100000
	./${TARGET} -w 15 -h 15 -b 50 -r output_15_15_50.txt -i 100000
	./${TARGET} -w 15 -h 15 -b 20 -r output_15_15_20.txt -i 100000
	./${TARGET} -w 9 -h 9 -b 50 -r output_9_9_50.txt -i 100000
	./${TARGET} -w 9 -h 9 -b 20 -r output_9_9_20.txt -i 100000
	gnuplot -c gnuplot.script

clean:
	$(RM) -rf $(BUILD_DIR)
