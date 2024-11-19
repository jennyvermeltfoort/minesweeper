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
    BOARD_STATE_NORMAL = 1,
    BOARD_STATE_SHOW_BOMB = 2,
    BOARD_STATE_DEAD = 4,
    BOARD_STATE_DONE = 8,
    BOARD_STATE_MASK = 0xF,
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

bool cell_is_state(const cell_t *const cell,
                   const cell_state_e state);
void cell_set_state(cell_t *const cell, const cell_state_e state);
void cell_unset_state(cell_t *const cell, const cell_state_e state);
void cell_toggle_state(cell_t *const cell, const cell_state_e state);

typedef enum BOARD_RETURN_T {
    BOARD_RETURN_OK = 1,
    BOARD_RETURN_NO_FLAGS = 2,
    BOARD_RETURN_IS_OPEN = 4,
    BOARD_RETURN_STOP = 8,
    BOARD_RETURN_IS_FLAG = 16,
} board_return_t;

typedef struct BOARD_INFO_T {
    int flag_count;
    int open_count;
    unsigned int state;
} board_info_t;

class Board {
   private:
    const unsigned int board_size_x;
    const unsigned int board_size_y;
    cell_t *const board_start = new cell_t;
    cell_t *board_cursor;
    board_info_t info;

    void set_cursor(cell_t *cursor);
    void grid_iterater(void (*func)(cell_t *cell));

   public:
    Board(const unsigned int size_x, const unsigned int size_y,
          const unsigned int bomb_count);
    ~Board(void);

    void grid_iterater(std::function<void(cell_info_t *const)> func);

    void print(void);

    board_info_t get_info(void) const;
    void set_info(board_info_t info);

    bool is_state(const board_state_e state) const;
    void set_state(const board_state_e state);
    void unset_state(const board_state_e state);
    void toggle_state(const board_state_e state);

    board_return_t cursor_set_open(void);
    board_return_t cursor_set_flag(void);
    void cursor_move_north(void);
    void cursor_move_south(void);
    void cursor_move_west(void);
    void cursor_move_east(void);
    void toggle_show_bomb(void);
};

#endif  // __GUARD_BOARD_H
