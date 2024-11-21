
#include <fstream>

#include "board.hpp"
#include "print.hpp"
#include "stack.hpp"

#ifndef __GUARD_INPUT_H
#define __GUARD_INPUT_H

class BoardHandler {
   private:
    BoardStack stack;
    BoardPrinter printer;

   public:
    BoardHandler(void);
    void user_init(const Board &board);
    bool user_parse_input(Board &board, const char c);
    void automated(const unsigned int width, const unsigned int height,
                   const unsigned int bomb_count, std::fstream &output_file,
                   unsigned int iterations);
};

#endif  // __GUARD_INPUT_H
