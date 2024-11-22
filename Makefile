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
	./${TARGET} -w 20 -h 20 -b 200 -r output_20_20_200.txt -i 10000
	./${TARGET} -w 15 -h 15 -b 100 -r output_15_15_100.txt -i 10000
	./${TARGET} -w 9 -h 9 -b 10 -r output_9_9_10.txt -i 10000
	gnuplot -c gnuplot.script

clean:
	$(RM) -rf $(BUILD_DIR)
