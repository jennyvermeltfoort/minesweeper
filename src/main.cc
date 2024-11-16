
#include <iostream>

#include "stack.h"
#include "world.h"

int main(void) {
    const unsigned int BOARD_SIZE_X = 20;
    const unsigned int BOARD_SIZE_y = 20;
    Board board(BOARD_SIZE_X, BOARD_SIZE_y, 10);
    board.cell_set_open(3,3);
    board.cell_set_flag(5,5);

    board.print();
}
