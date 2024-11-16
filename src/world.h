
#include "stack.h"

#ifndef __GUARD_WORLD_H
#define __GUARD_WORLD_H

typedef struct CELL_T cell_t;
typedef struct CELL_INFO_T {
    bool is_bomb;
    bool is_flag;
    bool is_open;
    unsigned int bomb_count;
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

typedef enum BOARD_RETURN_T {
	BOARD_RETURN_OK = 0,
	BOARD_RETURN_BOMB_CELL,
	BOARD_RETURN_NO_FLAGS,
} board_return_t;

class Board {
   private:
    const unsigned int board_size_x;
    const unsigned int board_size_y;
    cell_t * const board_start = new cell_t;
    int flag_count;

    cell_t *init_grid(void);
    void init_bomb(unsigned int bomb_count);
    cell_t *grid_get_cell(unsigned int x, unsigned int y);

   public:
    Board(const unsigned int size_x, const unsigned int size_y,
              const unsigned int bomb_count);
    ~Board(void);

    board_return_t cell_set_open(unsigned int x, unsigned int y);
    board_return_t cell_set_flag(unsigned int x, unsigned int y);

    void print();
};

#endif  // __GUARD_WORLD_H
