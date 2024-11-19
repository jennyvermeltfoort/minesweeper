
#include "board.hh"

#ifndef __GUARD_PRINT_H
#define __GUARD_PRINT_H

class BoardPrinter {
   private:
    Board* board;

   public:
    BoardPrinter(Board* board);
    void print(void);
};

#endif  // __GUARD_PRINT_H
