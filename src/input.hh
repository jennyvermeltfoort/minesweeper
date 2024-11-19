
#include "board.hh"
#include "print.hh"
#include "stack.hh"

#ifndef __GUARD_INPUT_H
#define __GUARD_INPUT_H

class BoardHandler {
   private:
    Board* board;
    BoardPrinter* printer;
    BoardStack* stack;

   public:
    BoardHandler(Board* const _board);
    ~BoardHandler(void);
    void parse_input(const char c, bool* const _is_end);
};

#endif  // __GUARD_INPUT_H
