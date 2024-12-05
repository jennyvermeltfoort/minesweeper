
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
