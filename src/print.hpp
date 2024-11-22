
#include "board.hpp"

#ifndef __GUARD_PRINT_H
#define __GUARD_PRINT_H

class BoardPrinter {
   public:
    static void print_frame(const Board &board);
    static void print(const Board &board);
};

#endif  // __GUARD_PRINT_H
