
#include "board.hpp"

#ifndef __GUARD_PRINT_H
#define __GUARD_PRINT_H

class BoardPrinter {
   public:
    void print_frame(const Board &board);
    void print(const Board &board);
};

#endif  // __GUARD_PRINT_H
