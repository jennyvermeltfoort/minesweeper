#include "world.h"

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
    cell->north = init_cell(cell, x, y + 1);
}

void CellBoard::init_cell_east(cell_t* cell, const unsigned int x,
                               const unsigned int y) {
    cell->east = init_cell(cell->south_east, x + 1, y);
    cell->east->west = cell;
    cell->east->south_west = cell->south;
    cell->north_east = cell->east->north;
    cell->south_east = cell->east->south;
}

void CellBoard::init_cell_west(cell_t* cell) {
    cell->north_west = cell->west->north;
    cell->east->west = cell;
    cell->south_east = cell->east->south;
}

void CellBoard::init_south_cell_east(cell_t* cell,
                                     const unsigned int x,
                                     const unsigned int y) {
    cell->east =
        init_cell(nullptr, x + 1,
                  y);  // no southern neighbour at the southern edge.
    cell->east->west = cell;
}

void CellBoard::init_south_cell_west(cell_t* cell) {
    cell->north_west = cell->west->north;
}

void CellBoard::init_north_cell_east(cell_t* cell,
                                     const unsigned int x,
                                     const unsigned int y) {
    cell->east = init_cell(cell->south_east, x + 1, y);
    cell->east->west = cell;
    cell->east->south_west = cell->south;
}

void CellBoard::init_north_cell_west(cell_t* cell) {
    cell->south_west = cell->west->south;
}

cell_t* CellBoard::init_cell(cell_t* cell_south, const unsigned int x,
                             const unsigned int y) {
    cell_t* cell = new cell_t;
    cell->south = cell_south;

    std::cout << "test" << std::endl;

    if (is_south_west_corner(x, y)) {
        init_south_cell_east(cell, x, y);
        init_cell_north(cell, x, y);

    } else if (is_south_east_corner(x, y)) {
        init_south_cell_west(cell);

    } else if (is_north_west_corner(x, y)) {
        init_north_cell_east(cell, x, y);

    } else if (is_north_east_corner(x, y)) {
        iinit_north_cell_west(cell);

    } else if (is_south_edge(x, y)) {
        init_cell_north(cell, x, y);
        init_south_cell_east(cell, x, y);
        init_south_cell_west(cell);

    } else if (is_north_edge(x, y)) {
        init_north_cell_east(cell, x, y);
        init_north_cell_west(cell);

    } else if (is_west_edge(x, y)) {
        init_cell_east(cell, x, y);
        init_cell_north(cell, x, y);

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

    while (cell_y->north != nullptr) {
        cell_y = cell_y->north;
    }

    while (cell_y->south != nullptr) {
        num = 100 * i++;
        cell_x = cell_y;

        while (cell_x->east != nullptr) {
            std::cout << +num++ << " ";
        }
        std::cout << std::endl;

        cell_y = cell_y->south;
    }
}

CellBoard::CellBoard(const unsigned int size_x,
                     const unsigned int size_y)
    : board_size_x(size_x), board_size_y(size_y) {
    start = init_cell(nullptr, 0, 0);
}

CellBoard::~CellBoard(void) { start = init_cell(nullptr, 0, 0); }

cell_t* CellBoard::init_cell_south(cell_t* cell_top, unsigned int x,
                                   unsigned int y) {
    cell_t* cell = nullptr;
    if (!is_north_edge(x, y)) {
        cell = new cell_t;
        cell->north = cell_top;
        cell->south = init_cell_bottom(cell);
    }
    return cell;
}

cell_t* CellBoard::init_cell_east(cell_t* cell_west, unsigned int x,
                                  unsigned int y) {
    cell_t* cell = nullptr;
    if (!is_east_edge(x, y)) {
        cell = new cell_t;
        cell->west = cell_west;
        cell->east = init_cell_east(cell);
    }
    return cell;
}

void CellBoard::set_cell_east(cell_t* cell) {
    if (cell != nullptr) {
        cell->north_west = cell->west->north;
        cell->north = cell->west->north_east;
        cell->south_west = cell->west->south;
        cell->south = cell->west->south_west;
        if (cell->east != nullptr) {
            cell->south_east = cell->east->south;
            cell->north_east = cell->east->north;
            set_cell_east(cell->east);
        }
    }
}

void CellBoard::set_cell_south(cell_t* cell) {
    if (cell != nullptr) {
        cell->north_west = cell->north->west;
        cell->west = cell->north->south_west;
        cell->north_east = cell->north->east;
        cell->east = cell->north->south_east;
        if (cell->south != nullptr) {
            cell->south_west = cell->south->west;
            cell->south_east = cell->south->east;
            set_cell_south(cell->east);
        }
    }
}

cell_t* CellBoard::init_cell_south_east(cell_t* cell_north_west,
                                        unsigned int x,
                                        unsigned int y) {
    cell_t* cell = new cell_t;
    cell->north_west = cell_north_west;

    cell->south = init_cell_south(cell);
    cell->east = init_cell_east(cell);

    if (!is_south_west_corner(x, y)) {
        cell->south_east = init_cell_south_east(cell);
    }

    cell->south_east->north = cell->west;
    cell->south_east->north_east = cell->west->west;

    cell->south_east->west = cell->south;
    cell->south_east->south_west = cell->south->south;

    set_cell_horizontal(cell->east);
    set_cell_vertical(cell->east);
}
