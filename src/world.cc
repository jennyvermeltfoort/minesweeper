#include "world.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

#define NEIGHBOUR_COUNT 8
typedef struct CELL_NEIGHBOURS_T {
	cell_t* array[NEIGHBOUR_COUNT];
} cell_neighbours_t;

int16_t min(int16_t n1, int16_t n2) { return (n1 < n2) ? n1 : n2; }

void populate_cell_east(cell_t* cell) {
    if (cell->north != nullptr) {
        cell->north_east = cell->north->east;
    }
    if (cell->south != nullptr) {
        cell->south_east = cell->south->east;
    }
}

void recursive_populate_row_all(cell_t* cell) {
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


int cell_count(cell_t* cell) {
    return (cell->north_west != nullptr) + (cell->north != nullptr) +
           (cell->north_east != nullptr) + (cell->west != nullptr) +
           (cell->east != nullptr) + (cell->south_west != nullptr) +
           (cell->south != nullptr) + (cell->south_east != nullptr);
}


cell_t* grid_walk_east(cell_t* cell,
                                    unsigned int steps) {
    while (steps-- > 0) {
        cell = cell->east;
    }
    return cell;
}

cell_t* grid_walk_south(cell_t* cell,
                                     unsigned int steps) {
    while (steps-- > 0) {
        cell = cell->south;
    }
    return cell;
}

cell_t* grid_walk_south_east(cell_t* cell,
                                          unsigned int steps) {
    while (steps-- > 0) {
        cell = cell->south_east;
    }
    return cell;
}

cell_neighbours_t cell_get_neighbour_array(cell_t * cell) {
	return { .array = {
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

cell_t * grid_walk_random_step(cell_t* cell) {
    	std::srand(std::time(nullptr));
	cell_neighbours_t neighbours = cell_get_neighbour_array(cell);
	unsigned int random_int = rand() % 8;

	do {
		cell = neighbours.array[random_int];
		random_int = (random_int + 1) % 8;
	} while (cell == nullptr);

	return cell;
}

void cell_make_bomb(cell_t * cell) {
	cell_neighbours_t neighbours = cell_get_neighbour_array(cell);
	for (unsigned int i = 0 ; i < NEIGHBOUR_COUNT ; i++) {
		cell_t * neighbour = neighbours.array[i];
		if (neighbour != nullptr) {
			neighbour->info.bomb_count++;
		}
	}
	cell->info.is_bomb = true;
}

void cell_open(cell_t * cell, cell_t * origin) {
	cell_neighbours_t neighbours = cell_get_neighbour_array(cell);
	cell->info.is_open = true;

	if (cell->info.bomb_count != 0) {
		return;
	}

	for (unsigned int i = 0 ; i < NEIGHBOUR_COUNT ; i++) {
		cell_t * neighbour = neighbours.array[i];
		if (neighbour != nullptr && neighbour->info.is_open == false 
				&& neighbour->info.is_flag == false) {
			cell_open(neighbour, cell);
		}
	}
}

board_return_t Board::cell_set_open(unsigned int x, unsigned int y) {
	cell_t* cell = grid_get_cell(x,y);
	if (cell->info.is_bomb == true) {
		return BOARD_RETURN_BOMB_CELL;
	}
	if (cell->info.is_open == true) {
		return BOARD_RETURN_IS_OPEN;
	}
	cell_open(cell, nullptr);
	return BOARD_RETURN_OK;
}

board_return_t Board::cell_set_flag(unsigned int x, unsigned int y) {
	cell_t* cell = grid_get_cell(x,y);
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

cell_t* Board::grid_get_cell(unsigned int x, unsigned int y) {
    cell_t* cell;
    unsigned int steps_diagonal;

    if (x >= board_size_x || y >= board_size_y) {
	    std::cout << "test" << std::endl;
        return nullptr;
    }

    steps_diagonal = min(x, y);
    cell = grid_walk_south_east(board_start, steps_diagonal);
    cell = grid_walk_east(cell, x - steps_diagonal);
    cell = grid_walk_south(cell, y - steps_diagonal);
    return cell;
}

cell_t* Board::init_grid(void) {
    cell_t* cell_first = board_start;
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

void Board::print(void) {
    cell_t* cell_x =board_start;
    cell_t* cell_y = board_start;

    while (cell_y != nullptr) {
        cell_x = cell_y;

        while (cell_x != nullptr) {
	    if (cell_x->info.is_open ) {
                 std::cout << cell_x->info.bomb_count;
	    } else if (cell_x->info.is_flag) {
                 std::cout << "f";
	     } else if (cell_x->info.is_bomb) {
                 std::cout << "x";
	    }else {
                 std::cout << ".";
             }
            cell_x = cell_x->east;
        }

        std::cout << std::endl;
        cell_y = cell_y->south;
    }
}

void Board::init_bomb(unsigned int bomb_count) {
    	std::srand(std::time(nullptr));
	while (bomb_count-- > 0) {
		unsigned int x = rand() % (board_size_x - 1);
		unsigned int y = rand() % (board_size_y - 1);
		cell_t *cell = grid_get_cell(x,y);
		while (cell->info.is_bomb == true) {
			cell = grid_walk_random_step(cell);
		}
		cell_make_bomb(cell);
	}
}

Board::Board(const unsigned int size_x,
                     const unsigned int size_y,
                     const unsigned int bomb_count)
    : board_size_x(size_x), board_size_y(size_y), flag_count(bomb_count) {
    init_grid();
    init_bomb(bomb_count);
}

Board::~Board(void) {}
