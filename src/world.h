
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
    const unsigned int board_size_x;
    const unsigned int board_size_y;
    void init_cell_top(cell_t **cell, cell_t *cell_south);
    void init_cell_north(cell_t **cell);
    void init_south_cell_east(cell_t **cell);
    void init_south_cell_west(cell_t **cell);
    void init_north_cell_east(cell_t **cell);
    void init_north_cell_west(cell_t **cell);
    void init_board(void);
    bool is_south_edge(const unsigned int x, const unsigned int y);
    bool is_north_edge(const unsigned int x, const unsigned int y);
    bool is_west_edge(const unsigned int x, const unsigned int y);
    bool is_east_edge(const unsigned int x, const unsigned int y);
    bool is_south_east_corner(const unsigned int x,
                              const unsigned int y);
    bool is_north_east_corner(const unsigned int x,
                              const unsigned int y);
    bool is_south_west_corner(const unsigned int x,
                              const unsigned int y);
    bool is_north_west_corner(const unsigned int x,
                              const unsigned int y);
    void init_middle_cell_west(cell_t *cell);
    void init_top_cell_west(cell_t *cell);
    void init_bottom_cell_west(cell_t *cell);
    void init_top_cell_east(cell_t *cell);
    void init_middle_cell_east(cell_t *cell);
    void init_bottom_cell_east(cell_t *cell);

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
