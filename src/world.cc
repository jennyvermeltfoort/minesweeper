#include "world.h"

void CellBoard::init_middle_cell_west(cell_t* cell) {
    cell->neighbour.north_west = new cell_t;
    cell->neighbour.west =
        cell->neighbour.south->neighbour.north_west;
    cell->neighbour.south_west =
        cell->neighbour.south->neighbour.west;
}

void CellBoard::init_top_cell_west(cell_t* cell) {
    cell->neighbour.west =
        cell->neighbour.south->neighbour.north_west;
    cell->neighbour.south_west =
        cell->neighbour.south->neighbour.west;
}

void CellBoard::init_bottom_cell_west(cell_t* cell) {
    cell->neighbour.north_west = new cell_t;
}

void CellBoard::init_top_cell_east(cell_t* cell) {
    cell->neighbour.east =
        cell->neighbour.south->neighbour.north_east;
    cell->neighbour.south_east =
        cell->neighbour.south->neighbour.east;
}

void CellBoard::init_middle_cell_east(cell_t* cell) {
    init_cell(cell->neighbour.north_east, cell->neighbour.east);
    cell->neighbour.east =
        cell->neighbour.south->neighbour.north_east;
    cell->neighbour.south_east =
        cell->neighbour.south->neighbour.east;
}

void CellBoard::init_bottom_cell_east(cell_t* cell) {
    init_cell(cell->neighbour.north_east, cell->neighbour.east);
}

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

void CellBoard::init_cell_north(cell_t** cell) {
    init_cell_top(&(*cell)->neighbour.north, *cell);
}

void CellBoard::init_cell_east(cell_t** cell) {
    (*cell)->neighbour.east = new cell_t;
    (*cell)->neighbour.east->neighbour.west = *cell;
    init_cell_top(&(*cell)->neighbour.north_east,
                  (*cell)->neighbour.east);
    (*cell)->neighbour.north_east =
        (*cell)->neighbour.east->neighbour.north;
    (*cell)->neighbour.south_east =
        (*cell)->neighbour.east->neighbour.south;
}

void CellBoard::init_south_cell_east(cell_t** cell) {
    (*cell)->neighbour.east = new cell_t;
    (*cell)->neighbour.east->neighbour.west = *cell;
    init_cell_top(&(*cell)->neighbour.north_east,
                  (*cell)->neighbour.east);
    (*cell)->neighbour.north_east =
        (*cell)->neighbour.east->neighbour.north;
}

void CellBoard::init_south_cell_west(cell_t** cell) {
    (*cell)->neighbour.north_west =
        (*cell)->neighbour.west->neighbour.north;
}

void CellBoard::init_north_cell_east(cell_t** cell) {
    (*cell)->neighbour.east = new cell_t;
    (*cell)->neighbour.east->neighbour.west = *cell;
    (*cell)->neighbour.east->neighbour.south_west =
        (*cell)->neighbour.south;
}

void CellBoard::init_north_cell_west(cell_t** cell) {
    (*cell)->neighbour.south_west =
        (*cell)->neighbour.west->neighbour.south;
}

void CellBoard::init_cell_top(cell_t** cell, cell_t* cell_south) {
    static unsigned int x = 0;
    static unsigned int y = 0;

    if (*cell != nullptr) {
        return;
    }

    *cell = new cell_t;
    (*cell)->neighbour.south = cell_south;

    if (is_south_west_corner(x, y)) {
        init_cell_north(cell);
        init_south_cell_east(cell);
    } else if (is_south_edge(x, y)) {
        init_cell_north(cell);
        init_south_cell_east(cell);
        init_south_cell_west(cell);
    } else if (is_south_east_corner(x, y)) {
        init_south_cell_west(cell);
    } else if (is_north_west_corner(x, y)) {
        init_north_cell_east(cell);
    } else if (is_north_edge(x, y)) {
        init_north_cell_east(cell);
        init_north_cell_west(cell);
    } else if (is_north_east_corner(x, y)) {
        init_north_cell_west(cell);
    } else {
        init_cell_north(cell);
    }

    // y++;
    //*cell->neighbour.south = cell_south;

    // if (x > 0 && y < size_y) {
    //     init_middle_cell_west(cell);
    // } else if (x > 0 && y >= size_y) {
    //     init_top_cell_west(cell);
    // }

    // if (x < size_x && y < size_y) {
    //     init_middle_cell_east(cell);
    // } else if (x < size_x && y >= size_y) {
    //     init_top_cell_east(cell);
    // }
}

void CellBoard::init_board(void) {}

CellBoard::CellBoard(int size_x, int size_y)
    : board_size_x(size_x), board_size_y(size_y) {
    start = nullptr;
    init_cell_top(&start, nullptr);
}
