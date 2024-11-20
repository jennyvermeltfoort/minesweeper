#include "print.hpp"

#include <functional>
#include <iostream>

const unsigned int color_frame_bg = 240;
const unsigned int color_open_bg = 241;
const unsigned int color_close_bg = 242;
const unsigned int color_dead_bg = 124;
const unsigned int color_done_bg = 40;
const unsigned int color_info_fg = 255;

const unsigned int CELL_WIDTH = 3;
const unsigned int CELL_HEIGHT = 1;
const unsigned int FRAME_POS_START_TEXT = CELL_WIDTH + 1;
const unsigned int FRAME_SIZE_INFO_Y = 3;
const unsigned int FRAME_SIZE_INPUT_Y = 6;

void print_reset_ansi(void) {
    std::cout << "\033[0;39;49m" << std::flush;
}

void print_reset_bg(void) { std::cout << "\033[49m" << std::flush; }

void print_cell(const unsigned int color, char c) {
    std::cout << "\033[48:5:" << +color << "m"
              << " " << c << " ";
}

void print_cell(const unsigned int color, int c) {
    std::cout << "\033[48:5:" << +color << "m"
              << " " << +c << " ";
}

void print_clear_screen(void) {
    std::cout << "\033[2J" << std::flush;
}

void print_set_cursor(unsigned int y, unsigned int x) {
    std::cout << "\033[" << +x << ";" << +y << "H" << std::flush;
}

void print_set_cursor_input(board_info_t* info) {
    const unsigned int START_INPUT_Y =
        CELL_HEIGHT * info->size.y + FRAME_SIZE_INFO_Y + 1;
    print_set_cursor(FRAME_POS_START_TEXT,
                     START_INPUT_Y + CELL_HEIGHT * 1);
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

void helper_board_state(const board_status_t* const status,
                        void (*done)(), void (*dead)()) {
    if (status->is_dead) {
        dead();
    } else if (status->is_done) {
        done();
    }
}

void print_info(const board_info_t* const info) {
    const unsigned int FRAME_POS_INFO_Y = CELL_HEIGHT * 2;
    const unsigned int START_FLAG = FRAME_POS_START_TEXT + 8;
    const unsigned int START_OPEN = FRAME_POS_START_TEXT + 21;
    const unsigned int START_STATE =
        CELL_WIDTH * (info->size.x + 1) - 8;

    print_set_background(color_frame_bg);
    print_set_cursor(FRAME_POS_START_TEXT, FRAME_POS_INFO_Y);

    std::cout << "Flags: [   ]; Open: [   ]";
    print_set_cursor(START_FLAG, CELL_HEIGHT * FRAME_POS_INFO_Y);
    std::cout << +info->status.flag_count << std::flush;
    print_set_cursor(START_OPEN, CELL_HEIGHT * FRAME_POS_INFO_Y);
    std::cout << +info->status.open_count << std::flush;

    print_set_cursor(START_STATE, FRAME_POS_INFO_Y);
    std::cout << "         " << std::flush;
    print_set_cursor(START_STATE, FRAME_POS_INFO_Y);
    helper_board_state(
        &info->status,
        []() { std::cout << " You won!" << std::flush; },
        []() { std::cout << "You died!" << std::flush; });
    print_reset_bg();
}

void print_cell(const Board& board, const cell_info_t* const info) {
    if (board.is_cell_info_cursor(info)) {
        print_set_inverse_fg_bg();
    }

    if (info->is_flag) {
        print_cell(color_close_bg, 'F');
    } else if (info->is_bomb && board.get_info().status.show_bomb) {
        print_cell(color_open_bg, 'X');
    } else if (info->is_open && info->bomb_count != 0) {
        print_cell(color_open_bg, +info->bomb_count);
    } else if (info->is_open) {
        print_cell(color_open_bg, ' ');
    } else {
        print_cell(color_close_bg, ' ');
    }
    std::cout << std::flush;

    if (board.is_cell_info_cursor(info)) {
        print_unset_inverse_fg_bg();
    }
}

void print_edge(unsigned int size) {
    while (size-- > 0) {
        std::cout << "   " << std::flush;
    }
}

void print_frame_block(const board_info_t* const info) {
    unsigned int size_index_y =
        info->size.y + FRAME_SIZE_INFO_Y + FRAME_SIZE_INPUT_Y;
    print_set_cursor(1, 1);
    while (size_index_y--) {
        print_set_background(color_frame_bg);
        print_edge(info->size.x + 2);
        print_reset_bg();
        std::cout << std::endl;
    }
}

void print_instructions(const board_info_t* const info) {
    const std::string instructions[3] = {
        "<h,j,k,l> left,down,up,right", "<space> open | <f> flag",
        "<p> previous"};
    const unsigned int START_INPUT_Y =
        CELL_HEIGHT * info->size.y + FRAME_SIZE_INFO_Y + 1;
    print_set_background(color_frame_bg);
    for (unsigned int i = 0; i < 3; i++) {
        print_set_cursor(FRAME_POS_START_TEXT,
                         START_INPUT_Y + CELL_HEIGHT * (2 + i));
        std::cout << instructions[i] << std::endl;
    }
    print_reset_bg();
}

void print_grid(const Board& board) {
    unsigned int index_y = 0;
    board_status_t status = board.get_info().status;
    helper_board_state(
        &status, []() { print_set_foreground(color_done_bg); },
        []() { print_set_foreground(color_dead_bg); });

    std::function<void(const cell_info_t* const)> func_x =
        [&board](const cell_info_t* const info) {
            print_cell(board, info);
        };
    std::function<void(const cell_info_t* const)> func_y =
        [&index_y](const cell_info_t* const) {
            const unsigned int START_BOARD = CELL_HEIGHT * 4;
            print_set_cursor(FRAME_POS_START_TEXT,
                             START_BOARD + CELL_HEIGHT * index_y++);
        };
    board.grid_iterater(func_x, func_y);
    print_set_foreground(color_info_fg);
}

void print_init(const Board& board) {
    board_info_t info = board.get_info();
    print_set_foreground(color_info_fg);
    print_clear_screen();
    print_frame_block(&info);
    print_instructions(&info);
}

void BoardPrinter::print(void) {
    board_info_t info = board.get_info();
    print_info(&info);
    print_grid(board);

    print_set_cursor_input(&info);
}

BoardPrinter::BoardPrinter(const Board& _board) : board(_board) {
    print_init(board);
    print();
}