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

void print_cell(const cell_t* const cell,
                const cell_t* const board_cursor, bool is_show_bomb) {
    if (cell == board_cursor) {
        print_set_inverse_fg_bg();
    }
    if (cell->info.is_open && cell->info.bomb_count != 0) {
        print_cell(color_open, +cell->info.bomb_count);
    } else if (cell->info.is_open && cell->info.bomb_count == 0) {
        print_cell(color_open, ' ');
    } else if (cell->info.is_flag) {
        print_cell(color_open, 'F');
    } else if (is_show_bomb == true && cell->info.is_bomb) {
        print_cell(color_open, 'X');
    } else {
        print_cell(color_close, ' ');
    }
    if (cell == board_cursor) {
        print_unset_inverse_fg_bg();
    }
    std::cout << std::flush;
}

void print_row(const cell_t* cell, const cell_t* const board_cursor,
               bool is_show_bomb) {
    print_cell(color_edge, ' ');
    while (cell != nullptr) {
        print_cell(cell, board_cursor, is_show_bomb);
        cell = cell->east;
    }
    print_cell(color_edge, ' ');
    std::cout << std::endl;
}

void print_grid(const cell_t* cell, const cell_t* const board_cursor,
                bool is_show_bomb, unsigned int edge_size_x) {
    print_edge(color_edge, edge_size_x);
    while (cell != nullptr) {
        print_row(cell, board_cursor, is_show_bomb);
        cell = cell->south;
    }
    print_edge(color_edge, edge_size_x);
}

void helper_board_state(bool is_done, bool is_dead, void (*done)(),
                        void (*dead)()) {
    if (is_dead == true) {
        dead();
    } else if (is_done == true) {
        done();
    }
}

void Board::print(void) {
    print_clear_screen();
    print_info(flag_count, open_count);

    helper_board_state(
        is_done, is_dead, []() { print_set_foreground(color_done); },
        []() { print_set_foreground(color_dead); });

    print_grid(board_start, board_cursor, is_show_bomb,
               board_size_x + 2);
    print_reset_ansi();

    helper_board_state(
        is_done, is_dead,
        []() { std::cout << "You won!" << std::endl; },
        []() { std::cout << "You died!" << std::endl; });
}
