#include "board.hh"

#include <cstddef>
#include <cstdlib>
#include <ctime>

#define NEIGHBOUR_COUNT 8
typedef struct CELL_NEIGHBOURS_T {
    cell_t* array[NEIGHBOUR_COUNT];
} cell_neighbours_t;

inline int16_t min(int16_t n1, int16_t n2) {
    return (n1 < n2) ? n1 : n2;
}

inline cell_neighbours_t cell_get_neighbour_array(cell_t* cell) {
    return {.array = {
                cell->north_west,
                cell->north,
                cell->north_east,
                cell->west,
                cell->east,
                cell->south_west,
                cell->south,
                cell->south_east,
            }};
}

void grid_iterater_board(cell_t* board_start,
                         void (*func)(cell_t* cell)) {
    cell_t* cell_y = board_start;
    while (cell_y != nullptr) {
        cell_t* cell_x = cell_y;
        cell_y = cell_y->south;
        while (cell_x != nullptr) {
            func(cell_x);
            cell_x = cell_x->east;
        }
    }
}

void Board::grid_iterater(
    std::function<void(cell_info_t* const info)> func_x,
    std::function<void(cell_info_t* const info)> func_y) {
    cell_t* cell_y = board_start;
    while (cell_y != nullptr) {
        cell_t* cell_x = cell_y;
        if (func_y != nullptr) {
            func_y(&cell_y->info);
        }
        cell_y = cell_y->south;
        while (cell_x != nullptr) {
            if (func_x != nullptr) {
                func_x(&cell_x->info);
            }
            cell_x = cell_x->east;
        }
    }
}

void grid_alloc_row(cell_t* cell_west, unsigned int size_x) {
    cell_t* cell_x = cell_west;
    while (size_x-- > 0) {
        cell_x->east = new cell_t;
        cell_x->east->west = cell_x;
        cell_x = cell_x->east;
    }
}

void grid_alloc(cell_t* cell_north_west, unsigned int size_y,
                unsigned int size_x) {
    cell_t* cell_y = cell_north_west;
    while (size_y-- > 0) {
        cell_t* cell_x = cell_y;
        cell_y->south = new cell_t;
        cell_y->south->north = cell_y;
        cell_y = cell_y->south;
        grid_alloc_row(cell_x, size_x);
    }
    grid_alloc_row(cell_y, size_x);
}

void cell_populate_east(cell_t* cell) {
    if (cell->north != nullptr) {
        cell->north_east = cell->north->east;
    }
    if (cell->south != nullptr) {
        cell->south_east = cell->south->east;
    }
}

void cell_populate_north(cell_t* cell) {
    cell->north_west = cell->west->north;
    cell->north = cell->west->north_east;

    if (cell->east != nullptr) {
        cell->north_east = cell->west->north_east->east;
    }
}

void cell_populate_south(cell_t* cell) {
    cell->south_west = cell->west->south;
    cell->south = cell->west->south_east;

    if (cell->east != nullptr) {
        cell->south_east = cell->west->south_east->east;
    }
}

void cell_populate(cell_t* cell) {
    if (cell->west == nullptr) {  // most western column
        return cell_populate_east(cell);
    }
    if (cell->west->north != nullptr) {
        cell_populate_north(cell);
    }
    if (cell->west->south != nullptr) {
        cell_populate_south(cell);
    }
}

cell_t* grid_walk_random_step(cell_t* cell) {
    cell_neighbours_t neighbours = cell_get_neighbour_array(cell);
    unsigned int random_int = rand() % 8;

    do {
        cell = neighbours.array[random_int];
        random_int = (random_int + 5) %
                     8;  // increment by 5 to find the
                         // a neighbour that is not a
                         // nullptr in a minimum of two steps.
    } while (cell == nullptr);

    return cell;
}

void cell_set_bomb(cell_t* cell) {
    cell_neighbours_t neighbours = cell_get_neighbour_array(cell);
    for (unsigned int i = 0; i < NEIGHBOUR_COUNT; i++) {
        cell_t* neighbour = neighbours.array[i];
        if (neighbour != nullptr) {
            neighbour->info.bomb_count++;
        }
    }
    cell_info_set_state(&cell->info, CELL_STATE_BOMB);
}

void bomb_init(cell_t* const board_start, unsigned int max_step_size,
               unsigned int bomb_count) {
    cell_t* cell = board_start;
    while (bomb_count-- > 0) {
        unsigned int steps = rand() % max_step_size;
        while (cell_info_is_state(&cell->info, CELL_STATE_BOMB) ||
               steps-- > 0) {
            cell = grid_walk_random_step(cell);
        }
        cell_set_bomb(cell);
    }
}

bool Board::is_cell_info_cursor(const cell_info_t* const info) const {
    const cell_t* const cell = reinterpret_cast<const cell_t* const>(
        info - offsetof(cell_t, info));
    return (cell == board_cursor);
}

void Board::set_cursor(cell_t* cursor) {
    if (cursor != nullptr) {
        board_cursor = cursor;
    }
}
void Board::cursor_move_north(void) {
    set_cursor(board_cursor->north);
}
void Board::cursor_move_south(void) {
    set_cursor(board_cursor->south);
}
void Board::cursor_move_west(void) { set_cursor(board_cursor->west); }
void Board::cursor_move_east(void) { set_cursor(board_cursor->east); }

int cell_recursive_open(cell_t* cell) {
    int counter_opened = 1;
    cell->info.state |= CELL_STATE_OPEN;

    if (cell->info.bomb_count != 0) {
        return counter_opened;
    }

    for (unsigned int i = 0; i < NEIGHBOUR_COUNT; i++) {
        cell_t* neighbour = cell_get_neighbour_array(cell).array[i];
        if (neighbour != nullptr &&
            !(neighbour->info.state & CELL_STATE_OPEN)) {
            counter_opened += cell_recursive_open(neighbour);
        }
    }

    return counter_opened;
}

void Board::cursor_set_open(void) {
    cell_info_t* const info = &board_cursor->info;

    if (cell_info_is_state(info, CELL_STATE_FLAG) ||
        is_state(BOARD_STATE_DEAD) || is_state(BOARD_STATE_DONE)) {
        return;
    }

    if (cell_info_is_state(info, CELL_STATE_BOMB)) {
        set_state(BOARD_STATE_DEAD);
        set_state(BOARD_STATE_SHOW_BOMB);
        return;
    }

    if (!(cell_info_is_state(info, CELL_STATE_OPEN))) {
        board_info.status.open_count -=
            cell_recursive_open(board_cursor);
    }

    if (board_info.status.open_count <= 0) {
        set_state(BOARD_STATE_DONE);
        set_state(BOARD_STATE_SHOW_BOMB);
    }
}

void Board::cursor_set_flag(void) {
    cell_info_t* const info = &board_cursor->info;

    if (is_state(BOARD_STATE_DEAD) || is_state(BOARD_STATE_DONE)) {
        return;
    }

    if (cell_info_is_state(info, CELL_STATE_FLAG)) {
        cell_info_unset_state(info, CELL_STATE_FLAG);
        board_info.status.flag_count++;
    } else if (!cell_info_is_state(info, CELL_STATE_OPEN)) {
        cell_info_set_state(info, CELL_STATE_FLAG);
        board_info.status.flag_count--;
    }
}

bool cell_info_is_state(const cell_info_t* const info,
                        const cell_state_e state) {
    return (info->state & ((state)&CELL_STATE_MASK));
}
void cell_info_set_state(cell_info_t* const info,
                         const cell_state_e state) {
    info->state |= ((state)&CELL_STATE_MASK);
}
void cell_info_unset_state(cell_info_t* const info,
                           const cell_state_e state) {
    info->state &= (~(state)&CELL_STATE_MASK);
}

board_info_t Board::get_info(void) const { return board_info; }
void Board::set_status(board_status_t _status) {
    board_info.status = _status;
}

bool Board::is_state(const board_state_e state) const {
    return (board_info.status.state & ((state)&BOARD_STATE_MASK));
}
void Board::set_state(const board_state_e state) {
    board_info.status.state |= ((state)&BOARD_STATE_MASK);
}
void Board::toggle_state(const board_state_e state) {
    board_info.status.state ^= ((state)&CELL_STATE_MASK);
}

Board::Board(const unsigned int size_x, const unsigned int size_y,
             const unsigned int bomb_count)
    : board_info({
          board_size_t{size_x, size_y},
          board_status_t{
              static_cast<int>(bomb_count),
              static_cast<int>(size_x * size_y - bomb_count), 0},
      }) {
    std::srand(std::time(nullptr));
    grid_alloc(board_start, board_info.size.y - 1,
               board_info.size.x - 1);
    grid_iterater_board(board_start, cell_populate);
    bomb_init(board_start, board_info.size.x, bomb_count);
    set_cursor(board_start);
}

Board::~Board(void) {
    grid_iterater_board(board_start, [](cell_t* cell) {
        if (cell->west != nullptr) {
            delete cell->west;
        }
    });
}
