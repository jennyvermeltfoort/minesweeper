
#include <iostream>

#include "board.hh"
#include "input.hh"
#include "stack.hh"

int main(void) {
    const unsigned int BOARD_SIZE_X = 20;
    const unsigned int BOARD_SIZE_y = 20;
    Board *board = new Board(BOARD_SIZE_X, BOARD_SIZE_y, 100);
    BoardInputHandler input_handler(board);
    bool is_end = false;

    board->print();
    while (is_end == false) {
        char c = std::cin.get();
        input_handler.parse_input(c, &is_end);
    }

    delete board;
}
