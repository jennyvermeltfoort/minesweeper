
#include "stack.h"

#ifndef __GUARD_WORLD_H
#define __GUARD_WORLD_H

#ifndef __GUARD_STACK_H
#define __GUARD_STACK_H

typedef struct CELL_T cell_t;
typedef struct CELL_NEIGHBOUR_LIST_T {
    cell_t *north_east;
    cell_t *north;
    cell_t *north_west;
    cell_t *west;
    cell_t *east;
    cell_t *south_east;
    cell_t *south;
    cell_t *south_west;
} cell_neighbour_list_t;
struct CELL_T {
    bool is_bomb;
    cell_neighbour_list_t neighbour;
};

class CellBoard {
   private:
    cell_t *start;
    void initialize_board(int size_x, int size_y);

   public:
    CellBoard(int size_x, int size_y);
    ~CellBoard(void);
};

#endif  // __GUARD_STACK_H

class World {
   private:
    CellBoard world;

   public:
    World(void);
    ~World(void);
};

#endif  // __GUARD_WORLD_H
