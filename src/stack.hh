
#include "board.hh"

#ifndef __GUARD_STACK_H
#define __GUARD_STACK_H

typedef struct CELL_ENCODED_T cell_encoded_t;
struct CELL_ENCODED_T {
    cell_info_t * board_encoded;
    cell_encoded_t * next_x;
    cell_encoded_t * next_y;
};

class BoardEncoded {
    private:
    cell_encoded_t * board_encoded_start;

    public:
    void encode(cell_t * board_start);
    void decode(cell_t * board_start);
};


class BoardStack {
    private:
        BoardEncoded *board_stack;

    public:
        void push(BoardEncoded*);
        BoardEncoded* pop(void);
        bool is_empty(void);
};







#endif // __GUARD_STACK_H
