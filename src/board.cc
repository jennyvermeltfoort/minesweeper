#include <cstdlib>
#include <ctime>

#include "board.hh"

#define NEIGHBOUR_COUNT 8
typedef struct CELL_NEIGHBOURS_T {
    cell_t* array[NEIGHBOUR_COUNT];
} cell_neighbours_t;

int16_t min(int16_t n1, int16_t n2) { return (n1 < n2) ? n1 : n2; }

cell_t* grid_walk_east(cell_t* const cell_start, unsigned int steps) {
    cell_t* cell = cell_start;
    while (steps-- > 0) {
        cell = cell->east;
    }
    return cell;
}

cell_t* grid_walk_south(cell_t* const cell_start,
                        unsigned int steps) {
    cell_t* cell = cell_start;
    while (steps-- > 0) {
        cell = cell->south;
    }
    return cell;
}

cell_t* grid_walk_south_east(cell_t* const cell_start,
                             unsigned int steps) {
    cell_t* cell = cell_start;
    while (steps-- > 0) {
        cell = cell->south_east;
    }
    return cell;
}

cell_neighbours_t cell_get_neighbour_array(cell_t* cell) {
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

cell_t* grid_get_cell(cell_t* board_start, unsigned int x,
                      unsigned int y) {
    unsigned int steps_diagonal = min(x, y);
    cell_t* cell;
    cell = grid_walk_south_east(board_start, steps_diagonal);
    cell = grid_walk_east(cell, x - steps_diagonal);
    cell = grid_walk_south(cell, y - steps_diagonal);
    return cell;
}

void cell_make_bomb(cell_t* cell) {
    cell_neighbours_t neighbours = cell_get_neighbour_array(cell);
    for (unsigned int i = 0; i < NEIGHBOUR_COUNT; i++) {
        cell_t* neighbour = neighbours.array[i];
        if (neighbour != nullptr) {
            neighbour->info.bomb_count++;
        }
    }
    cell->info.is_bomb = true;
}

int cell_recursive_open(cell_t* cell, cell_t* origin) {
    cell_neighbours_t neighbours = cell_get_neighbour_array(cell);
    cell->info.is_open = true;
    int cell_opened = 1;

    if (cell->info.bomb_count != 0) {
        return cell_opened;
    }

    for (unsigned int i = 0; i < NEIGHBOUR_COUNT; i++) {
        cell_t* neighbour = neighbours.array[i];
        if (neighbour != nullptr &&
            neighbour->info.is_open == false &&
            neighbour->info.is_flag == false) {
            cell_opened += cell_recursive_open(neighbour, cell);
        }
    }
    return cell_opened;
}

board_return_t Board::cursor_set_open(void) {
    cell_t* const cell = board_cursor;
    if (cell->info.is_flag == true) {
        return BOARD_RETURN_IS_FLAG;
    }
    if (cell->info.is_open == true) {
        return BOARD_RETURN_IS_OPEN;
    }
    if (cell->info.is_bomb == true) {
        is_dead = true;
        is_show_bomb = true;
        return BOARD_RETURN_STOP;
    }
    open_count -= cell_recursive_open(cell, nullptr);
    if (open_count <= 0) {
        is_done = true;
        is_show_bomb = true;
        return BOARD_RETURN_STOP;
    }
    return BOARD_RETURN_OK;
}

board_return_t Board::cursor_set_flag(void) {
    cell_t* const cell = board_cursor;
    if (cell->info.is_flag) {
        cell->info.is_flag = false;
        flag_count++;
        return BOARD_RETURN_OK;
    }
    if (flag_count < 0) {
        return BOARD_RETURN_NO_FLAGS;
    }
    if (cell->info.is_open == true) {
        return BOARD_RETURN_IS_OPEN;
    }
    cell->info.is_flag = true;
    flag_count--;
    return BOARD_RETURN_OK;
}

void grid_init_western_column(cell_t* cell_northern,
                              unsigned int size_y) {
    cell_t* cell_y = cell_northern;
    while (size_y-- > 0) {
        cell_y->south = new cell_t;
        cell_y->south->north = cell_y;
        cell_y = cell_y->south;
    }
}

void grid_init_rows(cell_t* cell_northern, unsigned int size_x) {
    cell_t* cell_y = cell_northern;
    while (cell_y != nullptr) {
        cell_t* cell_x = cell_y;
        unsigned int size_index = size_x;
        while (size_index-- > 0) {
            cell_x->east = new cell_t;
            cell_x->east->west = cell_x;
            cell_x = cell_x->east;
        }
        cell_y = cell_y->south;
    }
}

void cell_populate_east(cell_t* cell) {
    if (cell->north != nullptr) {
        cell->north_east = cell->north->east;
    }
    if (cell->south != nullptr) {
        cell->south_east = cell->south->east;
    }
}

void cell_recursive_populate_row_all(cell_t* cell) {
    cell_t* cell_west = cell->west;

    if (cell_west->north != nullptr) {
        cell->north_west = cell_west->north;
        cell->north = cell_west->north->east;
    }
    if (cell->east != nullptr && cell_west->north != nullptr) {
        cell->north_east = cell_west->north->east->east;
    }
    if (cell_west->south != nullptr) {
        cell->south_west = cell_west->south;
        cell->south = cell_west->south->east;
    }
    if (cell->east != nullptr && cell_west->south != nullptr) {
        cell->south_east = cell_west->south->east->east;
    }

    if (cell->east != nullptr) {
        cell_recursive_populate_row_all(cell->east);
    }
}

void grid_populate(cell_t* cell_northern) {
    cell_t* cell_y = cell_northern;
    while (cell_y != nullptr) {
        cell_populate_east(cell_y);
        cell_recursive_populate_row_all(cell_y->east);
        cell_y = cell_y->south;
    }
}

void Board::grid_init(void) {
    grid_init_western_column(board_start, board_size_y - 1);
    grid_init_rows(board_start, board_size_x - 1);
    grid_populate(board_start);
}

void Board::bomb_init(unsigned int bomb_count) {
    while (bomb_count-- > 0) {
        unsigned int x = rand() % (board_size_x - 1);
        unsigned int y = rand() % (board_size_y - 1);
        cell_t* cell = grid_get_cell(board_start, x, y);
        while (cell->info.is_bomb == true) {
            cell = grid_walk_random_step(cell);
        }
        cell_make_bomb(cell);
    }
}

void Board::set_cursor(cell_t* new_cursor) {
    if (new_cursor != nullptr) {
        board_cursor = new_cursor;
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
void Board::toggle_show_bomb(void) { is_show_bomb = !is_show_bomb; }

Board::Board(const unsigned int size_x, const unsigned int size_y,
             const unsigned int bomb_count)
    : board_size_x(size_x),
      board_size_y(size_y),
      flag_count(bomb_count),
      open_count(size_x * size_y - bomb_count),
      is_show_bomb(false) {
    std::srand(std::time(nullptr));
    grid_init();
    bomb_init(bomb_count);
    set_cursor(grid_get_cell(board_start, board_size_x / 2,
                             board_size_y / 2));
}

Board::~Board(void) {}
