/**
 * Prints the board to the CLI. Uses ANSI to do terminal manipulation. Is
 * compatible with terminals based on: Kitty, VTE, mintty, iTerm2 and Konsole.
 * The module draws a frame in which the game is played. Information related to
 * the board state is show here. When the user wins or loses the front color
 * becomes either green or red. The cursor is displayed by inversing the front
 * and background color which highlights the cursor cell. A cell is three
 * characters wide. The implimentation kinda sucks, but it works.
 */
#include "board.hpp"

#ifndef __GUARD_PRINT_H
#define __GUARD_PRINT_H

class BoardPrinter {
   public:
    static void print_frame(const Board &board);
    static void print(const Board &board);
};

#endif  // __GUARD_PRINT_H
