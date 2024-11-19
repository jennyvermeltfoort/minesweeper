#include <iostream>

#include "board.hh"

const unsigned int color_edge = 240;
const unsigned int color_open = 241;
const unsigned int color_close = 242;
const unsigned int color_dead = 124;
const unsigned int color_done = 40;

const unsigned int CELL_WIDTH = 3;
const unsigned int CELL_HEIGHT = 1;
void helper_board_state(unsigned int board_state, void (*done)(),
                        void (*dead)()) {
    if (board_state & BOARD_STATE_DEAD) {
        dead();
    } else if (board_state & BOARD_STATE_DONE) {
        done();
    }
}

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

void print_set_cursor(unsigned int y, unsigned int x) {
    std::cout << "\033[" << +x << ";" << +y << "H" << std::flush;
}

void print_info(const board_info_t* const info,
                const unsigned int board_size_x) {
    std::cout << "Flags: [   ]; Open: [   ]";
    print_set_cursor(CELL_WIDTH + 1 + 8, CELL_HEIGHT * 2);
    std::cout << +info->flag_count << std::flush;
    print_set_cursor(CELL_WIDTH + 1 + 21, CELL_HEIGHT * 2);
    std::cout << +info->open_count << std::flush;

    print_set_cursor(CELL_WIDTH * (board_size_x + 1) - 8,
                     CELL_HEIGHT * 2);
    helper_board_state(
        info->state, []() { std::cout << " You won!" << std::flush; },
        []() { std::cout << "You died!" << std::flush; });
}

void print_set_foreground(const unsigned int color) {
    std::cout << "\033[38:5:" << +color << "m" << std::flush;
}
void print_set_background(const unsigned int color) {
    std::cout << "\033[48:5:" << +color << "m" << std::flush;
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
    while (cell != nullptr) {
        print_cell(board, cell, board_cursor);
        cell = cell->east;
    }
}

void print_grid(const Board* board, const cell_t* cell,
                const cell_t* const board_cursor) {
    unsigned int index_y = 0;
    while (cell != nullptr) {
        print_set_cursor(CELL_WIDTH + 1,
                         CELL_HEIGHT * 4 + CELL_HEIGHT * index_y);
        print_row(board, cell, board_cursor);
        cell = cell->south;
        index_y++;
    }
}

void Board::print(void) {
    print_clear_screen();

    unsigned int size_index_y = board_size_y + 10;
    while (size_index_y--) {
        print_edge(color_edge, board_size_x + 2);
    }
    print_set_foreground(255);
    print_set_cursor(CELL_WIDTH + 1, CELL_HEIGHT * 2);
    print_info(&info, board_size_x);

    helper_board_state(
        info.state, []() { print_set_foreground(color_done); },
        []() { print_set_foreground(color_dead); });

    print_grid(this, board_start, board_cursor);
    print_reset_ansi();

    print_set_foreground(255);
    print_set_background(color_edge);
    print_set_cursor(CELL_WIDTH + 1, CELL_HEIGHT * board_size_y + 6);
    std::cout << "<h,j,k,l> left,down,up,right" << std::endl;
    print_set_cursor(CELL_WIDTH + 1, CELL_HEIGHT * board_size_y + 7);
    std::cout << "<space> open | <f> flag" << std::endl;
    print_set_cursor(CELL_WIDTH + 1, CELL_HEIGHT * board_size_y + 8);
    std::cout << "<p> previous" << std::endl;
    print_set_cursor(CELL_WIDTH + 1, CELL_HEIGHT * board_size_y + 5);
}
