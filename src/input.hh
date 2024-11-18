
#include "board.hh"

#ifndef __GUARD_INPUT_H
#define __GUARD_INPUT_H

class BoardInputHandler {
   private:
    Board* board;
    bool is_end;

   public:
    BoardInputHandler(Board* _board);
    void parse_input(char c, bool* _is_end);
};

#endif  // __GUARD_INPUT_H
