#include <cstdlib>
#include <ctime>

#include "board.hh"

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

void Board::grid_iterater(void (*func)(cell_t* cell)) {
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

void Board::grid_iterater(void (*func)(const cell_t* const cell)) {
    grid_iterater(reinterpret_cast<void (*)(cell_t*)>(func));
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
    cell_set_state(cell, CELL_STATE_BOMB);
}

void bomb_init(cell_t* const board_start, unsigned int max_step_size,
               unsigned int bomb_count) {
    cell_t* cell = board_start;
    while (bomb_count-- > 0) {
        unsigned int steps = rand() % max_step_size;
        while (cell_is_state(cell, CELL_STATE_BOMB) || steps-- > 0) {
            cell = grid_walk_random_step(cell);
        }
        cell_set_bomb(cell);
    }
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
void Board::toggle_show_bomb(void) {
    toggle_state(BOARD_STATE_SHOW_BOMB);
}

int cell_recursive_open(cell_t* cell, cell_t* origin) {
    int counter_opened = 1;
    cell->info.state |= CELL_STATE_OPEN;

    if (cell->info.bomb_count != 0) {
        return counter_opened;
    }

    for (unsigned int i = 0; i < NEIGHBOUR_COUNT; i++) {
        cell_t* neighbour = cell_get_neighbour_array(cell).array[i];
        if (neighbour != nullptr &&
            !(neighbour->info.state & CELL_STATE_OPEN)) {
            counter_opened += cell_recursive_open(neighbour, cell);
        }
    }

    return counter_opened;
}

board_return_t Board::cursor_set_open(void) {
    cell_t* const cell = board_cursor;

    if (cell_is_state(cell, CELL_STATE_FLAG)) {
        return BOARD_RETURN_IS_FLAG;
    }

    if (cell_is_state(cell, CELL_STATE_BOMB)) {
        set_state(BOARD_STATE_DEAD);
        set_state(BOARD_STATE_SHOW_BOMB);
        return BOARD_RETURN_STOP;
    }

    if (!(cell->info.state & CELL_STATE_OPEN)) {
        open_count -= cell_recursive_open(cell, nullptr);
    }

    if (open_count <= 0) {
        set_state(BOARD_STATE_DONE);
        set_state(BOARD_STATE_SHOW_BOMB);
        return BOARD_RETURN_STOP;
    }

    return BOARD_RETURN_OK;
}

board_return_t Board::cursor_set_flag(void) {
    cell_t* const cell = board_cursor;
    if (cell_is_state(cell, CELL_STATE_FLAG)) {
        cell_unset_state(cell, CELL_STATE_FLAG);
        flag_count++;
        return BOARD_RETURN_OK;
    }

    if (!cell_is_state(cell, CELL_STATE_OPEN)) {
        cell_set_state(cell, CELL_STATE_FLAG);
        flag_count--;
        return BOARD_RETURN_OK;
    }

    return BOARD_RETURN_IS_OPEN;
}

bool cell_is_state(const cell_t* const cell,
                   const cell_state_e state) {
    return (cell->info.state & ((state)&BOARD_STATE_MASK));
}
void cell_set_state(cell_t* const cell, const cell_state_e state) {
    cell->info.state |= ((state)&CELL_STATE_MASK);
}
void cell_unset_state(cell_t* const cell, const cell_state_e state) {
    cell->info.state &= (~(state)&CELL_STATE_MASK);
}
void cell_toggle_state(cell_t* const cell, board_state_e state) {
    cell->info.state ^= (~(state)&CELL_STATE_MASK);
}

bool Board::is_state(const board_state_e state) const {
    return (board_state & ((state)&BOARD_STATE_MASK));
}
void Board::set_state(const board_state_e state) {
    board_state |= ((state)&BOARD_STATE_MASK);
}
void Board::unset_state(const board_state_e state) {
    board_state &= (~(state)&CELL_STATE_MASK);
}
void Board::toggle_state(const board_state_e state) {
    board_state ^= ((state)&CELL_STATE_MASK);
}

Board::Board(const unsigned int size_x, const unsigned int size_y,
             const unsigned int bomb_count)
    : board_size_x(size_x),
      board_size_y(size_y),
      flag_count(bomb_count),
      open_count(size_x * size_y - bomb_count),
      board_state(BOARD_STATE_NORMAL) {
    std::srand(std::time(nullptr));

    grid_alloc(board_start, board_size_y - 1, board_size_x - 1);
    grid_iterater(cell_populate);
    bomb_init(board_start, board_size_x, bomb_count);
    set_cursor(board_start);
}

Board::~Board(void) {
    grid_iterater([](cell_t* cell) {
        if (cell->west != nullptr) {
            delete cell->west;
        }
    });
}
