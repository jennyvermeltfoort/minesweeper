#include <cstddef>
#include <cstdlib>
#include <ctime>

#include "board.hpp"
using namespace std;

#define NEIGHBOUR_COUNT 8
typedef struct CELL_NEIGHBOURS_T {
    cell_t* array[NEIGHBOUR_COUNT];
} cell_neighbours_t;

inline int16_t min(int16_t n1, int16_t n2) { return (n1 < n2) ? n1 : n2; }
inline cell_neighbours_t cell_get_neighbour_array(cell_t* cell) {
    return {.array = {cell->north_west, cell->north, cell->north_east,
                      cell->west, cell->east, cell->south_west, cell->south,
                      cell->south_east}};
}
cell_t* get_cell_from_info(cell_info_t* info) {
    return reinterpret_cast<cell_t*>(info - offsetof(cell_t, info));
}

void Board::grid_iterater(
    function<void(const cell_info_t* const info)> func_x,
    function<void(const cell_info_t* const info)> func_y) const {
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

void cell_populate(const cell_info_t* const info) {
    cell_t* cell = get_cell_from_info(const_cast<cell_info_t*>(info));
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

void cell_toggle_bomb(cell_t* cell) {
    cell_neighbours_t neighbours = cell_get_neighbour_array(cell);
    for (unsigned int i = 0; i < NEIGHBOUR_COUNT; i++) {
        cell_t* neighbour = neighbours.array[i];
        if (neighbour != nullptr) {
            neighbour->info.bomb_count += (cell->info.is_bomb) ? -1 : 1;
        }
    }
    cell->info.is_bomb = !cell->info.is_bomb;
}

void init_bomb(cell_t* board_start, unsigned int bomb_count,
               unsigned int limit) {
    cell_t* cell = board_start;
    while (bomb_count-- > 0) {
        unsigned int steps = rand() % limit;
        unsigned int direction = rand() % NEIGHBOUR_COUNT;
        while (steps-- > 0) {
            cell_neighbours_t neighbours = cell_get_neighbour_array(cell);
            if (neighbours.array[direction] != nullptr) {
                cell = neighbours.array[direction];
            } else {
                direction = (direction + 5) % NEIGHBOUR_COUNT;
            }
        }
        while (cell->info.is_bomb || cell->info.is_open) {
            cell_neighbours_t neighbours = cell_get_neighbour_array(cell);
            unsigned int random = rand() % NEIGHBOUR_COUNT;
            while (neighbours.array[random] == nullptr) {
                random = (random + 5) % NEIGHBOUR_COUNT;
            }
            cell = neighbours.array[random];
        }
        cell_toggle_bomb(cell);
    }
}

bool Board::is_cell_info_cursor(const cell_info_t* const info) const {
    const cell_t* const cell =
        get_cell_from_info(const_cast<cell_info_t*>(info));
    return (cell == board_cursor);
}

void cell_recursive_open(cell_t* cell, int& open_counter,
                         unsigned int& step_count) {
    step_count++;
    open_counter--;
    cell->info.is_open = true;
    if (cell->info.bomb_count != 0) {
        return;
    }
    for (unsigned int i = 0; i < NEIGHBOUR_COUNT; i++) {
        cell_t* neighbour = cell_get_neighbour_array(cell).array[i];
        if (neighbour != nullptr && !neighbour->info.is_open) {
            cell_recursive_open(neighbour, open_counter, step_count);
        }
    }
}

void Board::cursor_set_open(void) {
    cell_info_t* const info = &board_cursor->info;
    if (info->is_open || info->is_flag || board_info.status.is_dead ||
        board_info.status.is_done) {
        return;
    }
    if (info->is_bomb &&
        board_info.status.open_count ==
            static_cast<int>(board_info.size.x * board_info.size.y -
                             board_info.bomb_count)) {
        cell_toggle_bomb(board_cursor);
    }
    if (info->is_bomb) {
        board_info.status.is_dead = true;
        board_info.status.show_bomb = true;
        return;
    }
    cell_recursive_open(board_cursor, board_info.status.open_count,
                        board_info.step_count);
    if (board_info.status.open_count <= 0) {
        board_info.status.is_done = true;
        board_info.status.show_bomb = true;
    }
}

void Board::cursor_set_flag(void) {
    cell_info_t* const info = &board_cursor->info;
    if (board_info.status.is_dead || board_info.status.is_done) {
        return;
    }
    if (info->is_flag) {
        info->is_flag = false;
        board_info.status.flag_count++;
    } else if (!info->is_open) {
        info->is_flag = true;
        board_info.status.flag_count--;
    }
}

board_info_t Board::get_info(void) const { return board_info; }
void Board::set_status(board_status_t _status) { board_info.status = _status; }

void set_cursor(cell_t** board_cursor, cell_t* cursor) {
    if (cursor != nullptr && board_cursor != nullptr) {
        *board_cursor = cursor;
    }
}
void Board::cursor_move_north(void) {
    set_cursor(&board_cursor, board_cursor->north);
}
void Board::cursor_move_south(void) {
    set_cursor(&board_cursor, board_cursor->south);
}
void Board::cursor_move_west(void) {
    set_cursor(&board_cursor, board_cursor->west);
}
void Board::cursor_move_east(void) {
    set_cursor(&board_cursor, board_cursor->east);
}
void Board::toggle_show_bomb(void) {
    board_info.status.show_bomb = !board_info.status.show_bomb;
}

void Board::reinitialize(void) {
    grid_iterater(
        [](const cell_info_t* const info) {
            cell_t* cell = get_cell_from_info(const_cast<cell_info_t*>(info));
            cell->info = {};
        },
        nullptr);
    board_info.step_count = 0;
    board_info.status = {};
    board_info.status.open_count = static_cast<int>(
        board_info.size.x * board_info.size.y - board_info.bomb_count);
    init_bomb(board_start, board_info.bomb_count, board_info.size.x / 2);
    set_cursor(&board_cursor, board_start);
}

Board::Board(const unsigned int size_x, const unsigned int size_y,
             const unsigned int bomb_count)
    : board_start(new cell_t),
      board_info({
          board_size_t{size_x, size_y},
          board_status_t{static_cast<int>(bomb_count),
                         static_cast<int>(size_x * size_y - bomb_count), false,
                         false, false},
          bomb_count,
          0,
      }) {
    srand(time(nullptr));
    grid_alloc(board_start, board_info.size.y - 1, board_info.size.x - 1);
    grid_iterater(cell_populate, nullptr);
    init_bomb(board_start, board_info.bomb_count, board_info.size.x / 2);
    set_cursor(&board_cursor, board_start);
}

Board::~Board(void) {
    grid_iterater(
        [](const cell_info_t* const info) {
            cell_t* cell = get_cell_from_info(const_cast<cell_info_t*>(info));
            if (cell->west != nullptr) {
                delete cell->west;
            }
            if (cell->east == nullptr) {
                delete cell;
            }
        },
        nullptr);
}
