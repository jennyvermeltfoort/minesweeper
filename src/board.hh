#include <functional>

#ifndef __GUARD_BOARD_H
#define __GUARD_BOARD_H

typedef enum CELL_STATE_E {
    CELL_STATE_FLAG = 1,
    CELL_STATE_OPEN = 2,
    CELL_STATE_BOMB = 4,
    CELL_STATE_MASK = 0x7,
} cell_state_e;

typedef enum BOARD_STATE_E {
    BOARD_STATE_SHOW_BOMB = 1,
    BOARD_STATE_DEAD = 2,
    BOARD_STATE_DONE = 4,
    BOARD_STATE_MASK = 0x7,
} board_state_e;

typedef struct CELL_T cell_t;
typedef struct CELL_INFO_T {
    unsigned int state;
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

bool cell_info_is_state(const cell_info_t *const info,
                        const cell_state_e state);
void cell_info_set_state(cell_info_t *const info,
                         const cell_state_e state);
void cell_info_unset_state(cell_info_t *const info,
                           const cell_state_e state);

typedef struct BOARD_SIZE_T {
    unsigned int x;
    unsigned int y;
} board_size_t;
typedef struct BOARD_STATUS_T {
    int flag_count;
    int open_count;
    unsigned int state;
} board_status_t;
typedef struct BOARD_INFO_T {
    board_size_t size;
    board_status_t status;
} board_info_t;

class Board {
   private:
    cell_t *const board_start = new cell_t;
    cell_t *board_cursor;
    board_info_t board_info;

    void set_cursor(cell_t *cursor);

   public:
    Board(const unsigned int size_x, const unsigned int size_y,
          const unsigned int bomb_count);
    ~Board(void);

    void grid_iterater(
        std::function<void(cell_info_t *const)> func_x,
        std::function<void(cell_info_t *const)> func_y);

    board_info_t get_info(void) const;
    void set_status(board_status_t state);

    bool is_state(const board_state_e state) const;
    void set_state(const board_state_e state);
    void toggle_state(const board_state_e state);

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
