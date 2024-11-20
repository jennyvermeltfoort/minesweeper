
#include "board.hpp"

#ifndef __GUARD_PRINT_H
#define __GUARD_PRINT_H

class BoardPrinter {
   private:
    const Board& board;

   public:
    explicit BoardPrinter(const Board& board);
    void print(void);
    void print_frame(void);
};

#endif  // __GUARD_PRINT_H
