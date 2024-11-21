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

    // perform func for every cell in grid, for every cell in row from
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
