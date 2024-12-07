/**
 * A board is a grid of cells. Each cell has pointers to its neighbor
 * counterparts. A cursor, which is basically a pointer to a specific cell, is
 * stored. Through this cursor the user may change the state of the board, like
 * opening and flagging cells. The cursor can be moved.
 *
 * The board is created by first allocating all cells. This is done by starting
 * in the top left. Then allocating a row for each cell walking downwards, when
 * walking downwards the northern and southern cells are populated. When the row
 * is allocated the new cells are stored in the west and east neighbor pointer.
 * So we end up with a western column where each cell has a eastern, a northern
 * and a southern cell populated. And for each row all western and eastern
 * neihgbors are populated.
 *
 * After full allocation of the grid, all empty neighbors are populated. Since
 * all horizontal relations between the cells exists (west and east) it is safe
 * to find northern and southern neighbors through the western relation of each
 * cell. We start at the top of the western column and walk downwards, while
 * walking over each row from west to east, on the way populating northwest,
 * north, northeast, southwest, south, southeast.
 */
#include <functional>

#ifndef __GUARD_BOARD_H
#define __GUARD_BOARD_H

typedef struct CELL_T cell_t;
typedef struct CELL_INFO_T {
    bool is_flag;
    bool is_bomb;
    bool is_open;
    unsigned char bomb_count;
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

typedef struct BOARD_SIZE_T {
    unsigned int x;
    unsigned int y;
} board_size_t;
typedef struct BOARD_STATUS_T {
    int flag_count;
    int open_count;
    bool show_bomb;
    bool is_dead;
    bool is_done;
} board_status_t;
typedef struct BOARD_INFO_T {
    board_size_t size;
    board_status_t status;
    unsigned int bomb_count;
    unsigned int step_count;
} board_info_t;

class Board {
   private:
    cell_t *const board_start;
    cell_t *board_cursor;
    board_info_t board_info;

   public:
    Board(const unsigned int size_x, const unsigned int size_y,
          const unsigned int bomb_count);
    ~Board(void);

    // Perform func for every cell in grid, for every cell in row from
    // left to right from top to bottom.
    void grid_iterater(
        std::function<void(const cell_info_t *const)> func_x,
        std::function<void(const cell_info_t *const)> func_y) const;

    void set_cell_info(const cell_info_t *const, cell_info_t info);

    board_info_t get_info(void) const;
    void set_status(board_status_t state);
    void toggle_show_bomb(void);

    void reinitialize(void);

    void cursor_set_open(void);
    void cursor_set_flag(void);
    void cursor_move_north(void);
    void cursor_move_south(void);
    void cursor_move_west(void);
    void cursor_move_east(void);

    // the info pointer should be a member of a cell.
    bool is_cell_info_cursor(const cell_info_t *const info) const;
};

#endif  // __GUARD_BOARD_H
