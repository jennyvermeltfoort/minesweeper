
#include "board.hh"

#ifndef __GUARD_STACK_H
#define __GUARD_STACK_H

typedef struct CELL_ENCODED_T cell_encoded_t;
struct CELL_ENCODED_T {
    cell_info_t info;
    cell_encoded_t* next;
};

class BoardEncoded {
   private:
    cell_encoded_t* start = nullptr;
    board_info_t info;

   public:
    BoardEncoded(Board* board);
    ~BoardEncoded(void);
    void decode(Board* board);
};

typedef struct BOARD_STACK_CELL_T board_stack_cell_t;
struct BOARD_STACK_CELL_T {
    BoardEncoded* encoded;
    board_stack_cell_t* next;
};

class BoardStack {
   private:
    board_stack_cell_t* start;

   public:
    ~BoardStack(void);
    void push(BoardEncoded* encoded);
    BoardEncoded* pop(void);
    bool is_empty(void);
};

#endif  // __GUARD_STACK_H
