
#include "board.hh"
#include "stack.hh"

#ifndef __GUARD_INPUT_H
#define __GUARD_INPUT_H

class BoardInputHandler {
   private:
    Board* board;
    BoardStack* stack;

   public:
    BoardInputHandler(Board* _board);
    ~BoardInputHandler(void);
    void parse_input(char c, bool* _is_end);
};

#endif  // __GUARD_INPUT_H
