/**
 * Stores the board state into a encoded board. The encoded board only holds the
 * cell information, the pointer structure of each cell is lost. Storing the
 * pointer structure is not neccesary since its static. The board decoder just
 * places the cell information back into all cells of the board, changing the
 * board state to the stored board state. The stack is just a list of pointers
 * towards these encoded boards.
 */
#include <list>

#include "board.hpp"

#ifndef __GUARD_STACK_H
#define __GUARD_STACK_H

class BoardEncoded {
   private:
    std::list<cell_info_t>* llist = new std::list<cell_info_t>;
    board_info_t info;

   public:
    explicit BoardEncoded(const Board& board);
    ~BoardEncoded(void);
    void decode(Board& board);
};

class BoardStack {
   private:
    std::list<BoardEncoded*>* llist = new std::list<BoardEncoded*>;

   public:
    ~BoardStack(void);
    void push(BoardEncoded* encoded);
    BoardEncoded* pop(void);
    bool is_empty(void);
};

#endif  // __GUARD_STACK_H
