#include "world.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

int16_t min(int16_t n1, int16_t n2) { return (n1 < n2) ? n1 : n2; }

bool CellBoard::is_not_south_edge(const unsigned int y) {
    return (y < board_size_y);
}

bool CellBoard::is_not_east_edge(const unsigned int x) {
    return (x < board_size_x);
}
bool CellBoard::is_not_south_west_corner(const unsigned int x,
                                         const unsigned int y) {
    return is_not_south_edge(y) && is_not_east_edge(x);
}

void CellBoard::populate_cell_east(cell_t* cell) {
    if (cell->north != nullptr) {
        cell->north_east = cell->north->east;
    }
    if (cell->south != nullptr) {
        cell->south_east = cell->south->east;
    }
}

void CellBoard::recursive_populate_row_all(cell_t* cell) {
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
        recursive_populate_row_all(cell->east);
    }
}

cell_t* CellBoard::init_grid(void) {
    cell_t* cell_first = new cell_t;
    cell_t *cell_y = cell_first;

    for (int i = board_size_y - 1; i > 0 ; i--) {
        cell_y->south = new cell_t;
        cell_y->south->north = cell_y;
        cell_y = cell_y->south;
    }


    cell_y = cell_first;
    while (cell_y != nullptr) {
        cell_t* cell_x = cell_y;
	for (int i = board_size_x - 1 ; i > 0 ; i--) {
             cell_x->east = new cell_t;
             cell_x->east->west = cell_x;
             cell_x = cell_x->east;
        }
	cell_y = cell_y->south;
    }

    cell_y = cell_first;
    while(cell_y != nullptr) {
        populate_cell_east(cell_y);
        recursive_populate_row_all(cell_y->east);
	cell_y = cell_y->south;
    }
    
    return cell_first; // most north western cell
}

int cell_count(cell_t* cell) {
    return (cell->north_west != nullptr) + (cell->north != nullptr) +
           (cell->north_east != nullptr) + (cell->west != nullptr) +
           (cell->east != nullptr) + (cell->south_west != nullptr) +
           (cell->south != nullptr) + (cell->south_east != nullptr);
}

void CellBoard::print(void) {
    cell_t* cell_x =board_start;
    cell_t* cell_y = board_start;
    int i = 0;

    while (cell_y != nullptr) {
        cell_x = cell_y;

	std::cout << +i++  << ":";

        while (cell_x != nullptr) {
            std::cout << cell_count(cell_x);
            // if (cell_x->is_bomb) {
            //     std::cout << "x";
            // } else {
            //     std::cout << " ";
            // }
            cell_x = cell_x->east;
        }

        std::cout << std::endl;
        cell_y = cell_y->south;
    }
}

cell_t* CellBoard::grid_walk_east(cell_t* cell,
                                    unsigned int steps) {
    while (steps-- > 0) {
        cell = cell->east;
    }
    return cell;
}

cell_t* CellBoard::grid_walk_south(cell_t* cell,
                                     unsigned int steps) {
    while (steps-- > 0) {
        cell = cell->south;
    }
    return cell;
}

cell_t* CellBoard::grid_walk_south_east(cell_t* cell,
                                          unsigned int steps) {
    while (steps-- > 0) {
        cell = cell->south_east;
    }
    return cell;
}

cell_info_t* CellBoard::grid_get_cell_info(unsigned int x, unsigned int y) {
    cell_t* cell;
    unsigned int steps_diagonal;

    if (x > board_size_x || y > board_size_y) {
        return nullptr;
    }

    steps_diagonal = min(x, y);
    cell = grid_walk_south_east(board_start, steps_diagonal);
    cell = grid_walk_east(cell, x - steps_diagonal);
    cell = grid_walk_south(cell, y - steps_diagonal);
    return &cell->info;
}

/**
 * The board is composed by first allocating all the cells. The
 * algoritm recusivly builds the board and walks over the board
 * diagonally. Let x be the initial cell then f(x) does: then all
 * eastern cells of x are allocated. then all southern cells of x are
 * allocated. then f(x->south_east)
 *
 *  When the south eastern corner is visited, the algoritm walk
 */
CellBoard::CellBoard(const unsigned int size_x,
                     const unsigned int size_y,
                     const unsigned int count_bomb)
    : board_size_x(size_x), board_size_y(size_y) {
    std::srand(std::time(nullptr));
    board_start = init_grid();
    // cell_t* cell = raster_get_cell(5, 5);
    // cell->is_bomb = 1;
}

CellBoard::~CellBoard(void) {}
