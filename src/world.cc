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

cell_t* CellBoard::init_cell_all_east(cell_t* cell_west,
                                      const unsigned int x,
                                      const unsigned int y) {
    cell_t* cell = nullptr;
    if (is_not_east_edge(x)) {
        cell = new cell_t;
        cell->west = cell_west;
        cell->east = init_cell_all_east(cell, x + 1, y);
    }
    return cell;
}

cell_t* CellBoard::init_cell_all_south(cell_t* cell_north,
                                       const unsigned int x,
                                       const unsigned int y) {
    cell_t* cell = nullptr;
    if (is_not_south_edge(y)) {
        cell = new cell_t;
        cell->north = cell_north;
        cell->east = init_cell_all_east(cell, x, y);
        cell->south = init_cell_all_south(cell, x, y + 1);
    }
    return cell;
}

void CellBoard::populate_all_east(cell_t* cell) {
    cell_t* cell_west = cell->west;
    cell->north_west = cell_west->north;
    cell->north = cell_west->north->east;
    cell->north_east = cell_west->north->east->east;

    if (cell->west->south != nullptr) {
        cell->south_west = cell_west->south;
        cell->south = cell_west->south->west;
        cell->south_east = cell_west->south->east->east;
    }

    if (cell->east != nullptr) {
        populate_all_east(cell->east);
    }
}

void CellBoard::populate_south(cell_t* cell) {
    cell_t* cell_north = cell->north;
    cell->north_west = cell_north->west;
    cell->west = cell_north->south->west;
    cell->north_east = cell_north->east;
    cell->east = cell_north->south->east;
    cell->south_east = cell_north->east->south->south;
    cell->south_west = cell_north->west->south->south;
}

void CellBoard::populate_all(cell_t* cell) {
    populate_all_east(cell);

    if (cell->south != nullptr) {
        populate_south(cell->south);
        populate_all(cell->south);
    }
}

void CellBoard::populate_north_row(cell_t* cell) {
    cell_t* cell_west = cell->west;
    cell->south_west = cell_west->south;
    cell->south = cell_west->south->east;

    if (cell->east != nullptr) {
        cell->south_east = cell->south->east;
        populate_north_row(cell->east);
    }
}

void CellBoard::populate_west_column(cell_t* cell) {
    cell_t* cell_north = cell->north;
    cell->north_east = cell_north->east;

    if (cell->south != nullptr) {
        cell->south_east = cell->east->south;
        populate_west_column(cell->south);
    }
}

cell_t* CellBoard::init_raster(void) {
    cell_t* cell = new cell_t;

    cell->east = init_cell_all_east(cell, 0, 0);
    cell->south = init_cell_all_south(cell, 0, 0);
    cell->south_east = cell->south->east;
    populate_north_row(cell->east);
    populate_west_column(cell->south);
    populate_all(cell->south_east);

    return cell;
}

void CellBoard::print(void) {
    cell_t* cell_x;
    cell_t* cell_y = board_start;

    while (cell_y->south != nullptr) {
        cell_x = cell_y;

        while (cell_x->east != nullptr) {
            if (cell_x->is_bomb) {
                std::cout << "x";
            } else {
                std::cout << " ";
            }
            cell_x = cell_x->east;
        }

        std::cout << std::endl;
        cell_y = cell_y->south;
    }
}

cell_t* CellBoard::raster_walk_east(cell_t* cell,
                                    unsigned int steps) {
    while (steps-- > 0) {
        cell = cell->east;
    }
    return cell;
}

cell_t* CellBoard::raster_walk_south(cell_t* cell,
                                     unsigned int steps) {
    while (steps-- > 0) {
        cell = cell->south;
    }
    return cell;
}

cell_t* CellBoard::raster_walk_south_east(cell_t* cell,
                                          unsigned int steps) {
    while (steps-- > 0) {
        cell = cell->south_east;
    }
    return cell;
}

cell_t* CellBoard::raster_get_cell(unsigned int x, unsigned int y) {
    cell_t* cell = nullptr;
    unsigned int steps_diagonal;

    if (x > board_size_x || y > board_size_y) {
        return cell;
    }

    steps_diagonal = min(x, y);
    cell = raster_walk_south_east(board_start, steps_diagonal);
    cell = raster_walk_east(cell, x - steps_diagonal);
    cell = raster_walk_south(cell, y - steps_diagonal);
    return cell;
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
    board_start = init_raster();
    cell_t* cell = raster_get_cell(5, 5);
    cell->is_bomb = 1;
}

CellBoard::~CellBoard(void) {}
