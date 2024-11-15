include "world.h"

#include <iostream>

bool CellBoard::is_south_edge(const unsigned int x,
                              const unsigned int y) {
    return (y == 0);
}
bool CellBoard::is_north_edge(const unsigned int x,
                              const unsigned int y) {
    return (y == board_size_y);
}
bool CellBoard::is_west_edge(const unsigned int x,
                             const unsigned int y) {
    return (x == 0);
}
bool CellBoard::is_east_edge(const unsigned int x,
                             const unsigned int y) {
    return (x == board_size_x);
}
bool CellBoard::is_south_east_corner(const unsigned int x,
                                     const unsigned int y) {
    return is_south_edge(x, y) && is_east_edge(x, y);
}
bool CellBoard::is_north_east_corner(const unsigned int x,
                                     const unsigned int y) {
    return is_north_edge(x, y) && is_east_edge(x, y);
}
bool CellBoard::is_south_west_corner(const unsigned int x,
                                     const unsigned int y) {
    return is_south_edge(x, y) && is_west_edge(x, y);
}
bool CellBoard::is_north_west_corner(const unsigned int x,
                                     const unsigned int y) {
    return is_north_edge(x, y) && is_west_edge(x, y);
}

void CellBoard::init_cell_north(cell_t* cell, const unsigned int x,
                                const unsigned int y) {
    cell->near.north = init_cell(cell, x, y + 1);
}

void CellBoard::init_cell_east(cell_t* cell, const unsigned int x,
                               const unsigned int y) {
    cell->near.east = init_cell(cell->near.south_east, x + 1, y);
    cell->near.east->near.west = cell;
    cell->near.north_east = cell->near.east->near.north;
    cell->near.south_east = cell->near.east->near.south;
}

void CellBoard::init_cell_west(cell_t* cell) {
    cell->near.north_west = cell->near.west->near.north;
    cell->near.east->near.west = cell;
    cell->near.south_east = cell->near.east->near.south;
}

void CellBoard::init_south_cell_east(cell_t* cell,
                                     const unsigned int x,
                                     const unsigned int y) {
    cell->near.east =
        init_cell(nullptr, x + 1,
                  y);  // no southern neighbour at the southern edge.
    cell->near.east->near.west = cell;
}

void CellBoard::init_south_cell_west(cell_t* cell) {
    cell->near.north_west = cell->near.west->near.north;
}

void CellBoard::init_north_cell_east(cell_t* cell,
                                     const unsigned int x,
                                     const unsigned int y) {
    cell->near.east = init_cell(cell->near.south_east, x + 1, y);
    cell->near.east->near.west = cell;
    cell->near.east->near.south_west = cell->near.south;
}

void CellBoard::init_north_cell_west(cell_t* cell) {
    cell->near.south_west = cell->near.west->near.south;
}

cell_t* CellBoard::init_cell(cell_t* cell_south, const unsigned int x,
                             const unsigned int y) {
    cell_t* cell = new cell_t;
    cell->near.south = cell_south;

    std::cout << "test" << std::endl;

    if (is_south_west_corner(x, y)) {
        init_cell_north(cell, x, y);
        init_south_cell_east(cell, x, y);
    } else if (is_south_east_corner(x, y)) {
        init_south_cell_west(cell);
    } else if (is_north_west_corner(x, y)) {
        init_north_cell_east(cell, x, y);
    } else if (is_north_east_corner(x, y)) {
        init_north_cell_west(cell);
    } else if (is_south_edge(x, y)) {
        init_cell_north(cell, x, y);
        init_south_cell_east(cell, x, y);
        init_south_cell_west(cell);
    } else if (is_north_edge(x, y)) {
        init_north_cell_east(cell, x, y);
        init_north_cell_west(cell);
    } else if (is_west_edge(x, y)) {
        init_cell_north(cell, x, y);
        init_cell_east(cell, x, y);
    } else if (is_east_edge(x, y)) {
        init_cell_north(cell, x, y);
        init_cell_west(cell);
    } else {
        init_cell_north(cell, x, y);
        init_cell_east(cell, x, y);
        init_cell_west(cell);
    }

    return cell;
}

void CellBoard::print(void) {
    cell_t* cell_x;
    cell_t* cell_y = start;
    int num;
    int i = 1;

    while (cell_y->near.north != nullptr) {
        cell_y = cell_y->near.north;
    }

    while (cell_y->near.south != nullptr) {
        num = 100 * i++;
        cell_x = cell_y;

        while (cell_x->near.east != nullptr) {
            std::cout << +num++ << " ";
        }
        std::cout << std::endl;

        cell_y = cell_y->near.south;
    }
}

CellBoard::CellBoard(const unsigned int size_x,
                     const unsigned int size_y)
    : board_size_x(size_x), board_size_y(size_y) {
    start = init_cell(nullptr, 0, 0);
}

CellBoard::~CellBoard(void) { start = init_cell(nullptr, 0, 0); }
