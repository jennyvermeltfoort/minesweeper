#include "world.h"

#include <iostream>

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

cell_t* CellBoard::init_cell_south(cell_t* cell_top,
                                   const unsigned int x,
                                   const unsigned int y) {
    cell_t* cell = nullptr;
    if (is_not_south_edge(y)) {
        cell = new cell_t;
        cell->x = x;
        cell->y = y + 1;
        cell->north = cell_top;
        cell->south = init_cell_south(cell, x, y + 1);
    }
    return cell;
}

cell_t* CellBoard::init_cell_east(cell_t* cell_west,
                                  const unsigned int x,
                                  const unsigned int y) {
    cell_t* cell = nullptr;
    if (is_not_east_edge(x)) {
        cell = new cell_t;
        cell->x = x + 1;
        cell->y = y;
        cell->west = cell_west;
        cell->east = init_cell_east(cell, x + 1, y);
    }
    return cell;
}

void CellBoard::populate_cell_east(cell_t* cell) {
    if (cell != nullptr) {
        cell->north_west = cell->west->north;
        cell->north = cell->west->north_east;
        cell->south_west = cell->west->south;
        cell->south = cell->west->south_west;
        if (cell->west->south_east != nullptr) {
            cell->south_east = cell->west->south_east->east;
        }
        if (cell->west->north_east != nullptr) {
            cell->north_east = cell->west->north_east->east;
        }
        populate_cell_east(cell->east);
    }
}

void CellBoard::populate_cell_south(cell_t* cell) {
    if (cell != nullptr) {
        cell->north_west = cell->north->west;
        cell->west = cell->north->south_west;
        cell->north_east = cell->north->east;
        cell->east = cell->north->south_east;
        if (cell->north->south_west != nullptr) {
            cell->south_west = cell->north->south_west->south;
        }
        if (cell->north->south_east != nullptr) {
            cell->south_east = cell->north->south_east->south;
        }
        populate_cell_south(cell->south);
    }
}

void CellBoard::populate_cell_south_east(cell_t* cell,
                                         cell_t* cell_east,
                                         cell_t* cell_south) {
    cell->north = cell_east;
    cell->north_east = cell_east->east;
    cell->west = cell_south;
    cell->south_west = cell_south->south;
    populate_cell_south(cell_south);
    populate_cell_east(cell_east);
}

cell_t* CellBoard::init_cell_south_east(cell_t* cell_north_west,
                                        const unsigned int x,
                                        const unsigned int y) {
    cell_t* cell = new cell_t;
    cell->x = x;
    cell->y = y;
    cell->north_west = cell_north_west;

    if (is_not_south_west_corner(x, y)) {
        cell->east = init_cell_east(cell, x, y);
        cell->south = init_cell_south(cell, x, y);
        cell->south_east = init_cell_south_east(cell, x + 1, y + 1);
        populate_cell_south_east(cell->south_east, cell->east,
                                 cell->south);
    }

    return cell;
}
#include <iomanip>
void CellBoard::print(void) {
    cell_t* cell_x;
    cell_t* cell_y = start;
    int num;
    int i = 1;
    std::cout.width(10);
    while (cell_y->south != nullptr) {
        num = 100 + 10 * i++;
        cell_x = cell_y;

        std::cout << std::right << std::setw(2) << +cell_x->y << ":";

        while (cell_x->east != nullptr) {
            std::cout << std::setw(2) << +cell_x->x << "";
            cell_x = cell_x->east;
        }

        std::cout << std::endl;

        cell_y = cell_y->south;
    }
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
                     const unsigned int size_y)
    : board_size_x(size_x), board_size_y(size_y) {
    start = init_cell_south_east(nullptr, 0, 0);
    populate_cell_east(start->east);
    populate_cell_south(start->south);
}

CellBoard::~CellBoard(void) {}
