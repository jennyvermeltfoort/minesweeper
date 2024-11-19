
#include <iostream>

#include "board.hh"
#include "input.hh"
#include "print.hh"
#include "stack.hh"

int main(void) {
    const unsigned int BOARD_SIZE_X = 20;
    const unsigned int BOARD_SIZE_y = 20;
    Board *board = new Board(BOARD_SIZE_X, BOARD_SIZE_y, 20);
    BoardHandler input_handler(board);
    bool is_end = false;

    while (is_end == false) {
        char c = std::cin.get();
        input_handler.parse_input(c, &is_end);
    }

    delete board;
}
