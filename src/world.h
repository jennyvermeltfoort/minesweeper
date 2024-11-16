
#include "stack.h"

#ifndef __GUARD_WORLD_H
#define __GUARD_WORLD_H

typedef struct CELL_T cell_t;
typedef struct CELL_INFO_T {
    bool is_bomb;
} cell_info_t;

struct CELL_T {
    cell_info_t info;
    cell_t *north_east;
    cell_t *north;
    cell_t *north_west;
    cell_t *west;
    cell_t *east;
    cell_t *south_east;
    cell_t *south;
    cell_t *south_west;
};

class CellBoard {
   private:
    cell_t *board_start;
    const unsigned int board_size_x;
    const unsigned int board_size_y;
    cell_t *init_grid(void);
    void populate_cell_east(cell_t *cell);
    void recursive_populate_row_all(cell_t *cell);
    bool is_not_south_edge(const unsigned int y);
    bool is_not_east_edge(const unsigned int x);
    bool is_not_south_west_corner(const unsigned int x,
                                  const unsigned int y);
    cell_t *grid_walk_east(cell_t *cell, unsigned int steps);
    cell_t *grid_walk_south(cell_t *cell, unsigned int steps);
    cell_t *grid_walk_south_east(cell_t *cell, unsigned int steps);
    cell_info_t *grid_get_cell_info(unsigned int x, unsigned int y);

   public:
    CellBoard(const unsigned int size_x, const unsigned int size_y,
              const unsigned int count_bomb);
    ~CellBoard(void);

    void print();
};

class World {
   private:
    CellBoard world;

   public:
    World(void);
    ~World(void);
};

#endif  // __GUARD_WORLD_H
