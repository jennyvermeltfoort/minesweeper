
#include <iostream>

#include "stack.h"
#include "world.h"

int main(void) {
    const unsigned int BOARD_SIZE_X = 10;
    const unsigned int BOARD_SIZE_y = 10;
    CellBoard board(BOARD_SIZE_X, BOARD_SIZE_y);

    board.print();
}
