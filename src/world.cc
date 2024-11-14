#include "world.h"

void CellBoard::add_west(cell_t* initial, int y, int x) {
    if (x <= 0) {
        return;
    }

    initial->neighbour->west = new cell_t;
    add_north(initial->neighbour->north, y--, x);
    add_west(initial->neighbour->north, y, x--);
}

void CellBoard::add_north(cell_t* initial, int y, int x) {
    if (y <= 0) {
        return;
    }

    cell_t* new_cell = new cell_t;

    if (initial->neighbour->north_west != nullptr) {
        new_cell->neighbour->south_west =
            initial->neighbour->north_west;
    }

    if (initial->neighbour->north_east != nullptr) {
        new_cell->neighbour->south_east =
            initial->neighbour->north_east;
    }

    new_cell->neighbour->south = initial;
    initial->neighbour->north = new_cell;
    add_north(initial->neighbour->north, y--, x);
    add_west(initial->neighbour->north, y, x--);
}

void CellBoard::make_board(int size_x, int size_y) {
    cell_t cell_x = new cell_t;
    cell_t cell_y = cell_x;

    for (int y = size_y; y > 0; y--) {
        for (int x = size_x; x > 0; x--) {
            cell_x->neighbour->= cell_t;
        }
        cell_y->neighbour->north = new cell_t;
        cell_x = cell_y = cell_y->neighbour->north;
    }
}

CellBoard::CellStack(int size_x, int size_y) {
    initialize_board(size_x, size_y);
}
