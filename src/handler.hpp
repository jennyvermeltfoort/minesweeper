/**
 * Basically combines all modules into a functional game. This is thus the
 * interface through which the user operates the game. The user may play
 * manually or can configure a robot(automated) to play. User input is mapped to
 * functions in the board and stack. When the user performs an open or a flag
 * then the board state is stored into the stack, otherwise it does not. When
 * the robot plays the board state is not stored.
 */
#include <fstream>

#include "board.hpp"
#include "print.hpp"
#include "stack.hpp"

#ifndef __GUARD_INPUT_H
#define __GUARD_INPUT_H

class BoardHandler {
   private:
    BoardStack stack = BoardStack();
    static const BoardPrinter printer;

   public:
    void user_init(const Board &board);
    // returns true when the board is finished, so it expects to not be called
    // again, unless it is to restore the previous state.
    bool user_parse_input(Board &board, const char c);
    void automated(const unsigned int width, const unsigned int height,
                   const unsigned int bomb_count, std::fstream &output_file,
                   unsigned int iterations);
};

#endif  // __GUARD_INPUT_H
