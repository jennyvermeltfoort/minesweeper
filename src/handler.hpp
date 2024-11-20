
#include "board.hpp"
#include "print.hpp"
#include "stack.hpp"

#ifndef __GUARD_INPUT_H
#define __GUARD_INPUT_H

class BoardHandler {
   private:
    Board& board;
    BoardStack stack;
    BoardPrinter printer;
    bool printer_enabled;

   public:
    explicit BoardHandler(Board& board);
    void print_frame(void);
    void parse_input(const char c, bool* const is_end);
};

#endif  // __GUARD_INPUT_H
