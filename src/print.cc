#include <iostream>

#include "board.hh"

const unsigned int color_edge = 240;
const unsigned int color_open = 241;
const unsigned int color_close = 242;
const unsigned int color_dead = 124;
const unsigned int color_done = 40;

void print_edge(const unsigned int color, unsigned int size) {
    std::cout << "\033[48:5:" << +color << "m";
    while (size-- > 0) {
        std::cout << "   " << std::flush;
    }
    std::cout << std::endl;
}

void print_reset_ansi(void) {
    std::cout << "\033[0m"
              << "\033[39;49m" << std::flush;
}

void print_cell(const unsigned int color, char c) {
    std::cout << "\033[48:5:" << +color << "m"
              << " " << c << " ";
}

void print_cell(const unsigned int color, unsigned int n) {
    std::cout << "\033[48:5:" << +color << "m"
              << " " << +n << " ";
}

void print_clear_screen(void) {
    std::cout << "\033[2J" << std::flush;
    std::cout << "\033[" << 1 << ";" << 1 << "H" << std::flush;
}

void print_info(unsigned int flag_count, unsigned int open_count) {
    std::cout << "Flags: [" << flag_count << "]; Open: ["
              << open_count << "]" << std::endl;
}

void print_set_foreground(const unsigned int color) {
    std::cout << "\033[38:5:" << +color << "m" << std::flush;
}

void print_set_inverse_fg_bg(void) {
    std::cout << "\033[7m" << std::flush;
}

void print_unset_inverse_fg_bg(void) {
    std::cout << "\033[27m" << std::flush;
}

void print_cell(const Board* board, const cell_t* const cell,
                const cell_t* const board_cursor) {
    if (cell == board_cursor) {
        print_set_inverse_fg_bg();
    }

    if (cell_is_state(cell, CELL_STATE_FLAG)) {
        print_cell(color_close, 'F');
    } else if (cell_is_state(cell, CELL_STATE_BOMB) &&
               board->is_state(BOARD_STATE_SHOW_BOMB)) {
        print_cell(color_open, 'X');
    } else if (cell_is_state(cell, CELL_STATE_OPEN) &&
               cell->info.bomb_count != 0) {
        print_cell(color_open, +cell->info.bomb_count);
    } else if (cell_is_state(cell, CELL_STATE_OPEN)) {
        print_cell(color_open, ' ');
    } else {
        print_cell(color_close, ' ');
    }
    std::cout << std::flush;

    if (cell == board_cursor) {
        print_unset_inverse_fg_bg();
    }
}

void print_row(const Board* board, const cell_t* cell,
               const cell_t* const board_cursor) {
    print_cell(color_edge, ' ');
    while (cell != nullptr) {
        print_cell(board, cell, board_cursor);
        cell = cell->east;
    }
    print_cell(color_edge, ' ');
    std::cout << std::endl;
}

void print_grid(const Board* board, const cell_t* cell,
                const cell_t* const board_cursor,
                unsigned int edge_size_x) {
    print_edge(color_edge, edge_size_x);
    while (cell != nullptr) {
        print_row(board, cell, board_cursor);
        cell = cell->south;
    }
    print_edge(color_edge, edge_size_x);
}

void helper_board_state(unsigned int board_state, void (*done)(),
                        void (*dead)()) {
    if (board_state & BOARD_STATE_DEAD) {
        dead();
    } else if (board_state & BOARD_STATE_DONE) {
        done();
    }
}

void Board::print(void) {
    print_clear_screen();
    print_info(flag_count, open_count);

    helper_board_state(
        board_state, []() { print_set_foreground(color_done); },
        []() { print_set_foreground(color_dead); });

    print_grid(this, board_start, board_cursor, board_size_x + 2);
    print_reset_ansi();

    helper_board_state(
        board_state, []() { std::cout << "You won!" << std::endl; },
        []() { std::cout << "You died!" << std::endl; });
}
