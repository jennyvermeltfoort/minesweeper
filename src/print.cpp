#include <functional>
#include <iostream>

#include "print.hpp"
using namespace std;

const unsigned int color_frame_bg = 240;
const unsigned int color_open_bg = 241;
const unsigned int color_close_bg = 242;
const unsigned int color_dead_bg = 124;
const unsigned int color_done_bg = 40;
const unsigned int color_info_fg = 255;
const unsigned int CELL_WIDTH = 3;
const unsigned int CELL_HEIGHT = 1;
const unsigned int FRAME_POS_START_TEXT = CELL_WIDTH + 1;
const unsigned int FRAME_SIZE_INFO_Y = 4;
const unsigned int FRAME_SIZE_INPUT_Y = 6;

void set_inverse_fg_bg(void) { cout << "\033[7m" << flush; }
void unset_inverse_fg_bg(void) { cout << "\033[27m" << flush; }
void reset_ansi(void) { cout << "\033[0;39;49m" << flush; }
void reset_bg(void) { cout << "\033[49m" << flush; }
void clear_screen(void) { cout << "\033[2J" << flush; }
void print_cell(const unsigned int color, char c) {
    cout << "\033[48:5:" << +color << "m"
         << " " << c << " ";
}
void print_cell(const unsigned int color, int c) {
    cout << "\033[48:5:" << +color << "m"
         << " " << +c << " ";
}
void set_cursor(unsigned int y, unsigned int x) {
    cout << "\033[" << +x << ";" << +y << "H" << flush;
}
void set_cursor_input(board_info_t* info) {
    const unsigned int START_INPUT_Y =
        CELL_HEIGHT * info->size.y + FRAME_SIZE_INFO_Y + 1;
    set_cursor(FRAME_POS_START_TEXT, START_INPUT_Y + CELL_HEIGHT * 1);
}
void set_foreground(const unsigned int color) {
    cout << "\033[38:5:" << +color << "m" << flush;
}
void set_background(const unsigned int color) {
    cout << "\033[48:5:" << +color << "m" << flush;
}

void helper_board_state(const board_status_t* const status, void (*done)(),
                        void (*dead)()) {
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
    const unsigned int START_STEP = FRAME_POS_START_TEXT + 34;
    const unsigned int START_STATE = CELL_WIDTH * (info->size.x + 1) - 8;
    set_background(color_frame_bg);
    set_cursor(FRAME_POS_START_TEXT, FRAME_POS_INFO_Y);
    cout << "Flags: [   ]; Open: [   ]; Step: [   ]";
    set_cursor(START_FLAG, CELL_HEIGHT * FRAME_POS_INFO_Y);
    cout << +info->status.flag_count << flush;
    set_cursor(START_OPEN, CELL_HEIGHT * FRAME_POS_INFO_Y);
    cout << +info->status.open_count << flush;
    set_cursor(START_STEP, CELL_HEIGHT * FRAME_POS_INFO_Y);
    cout << +info->step_count << flush;
    set_cursor(START_STATE, FRAME_POS_INFO_Y + 1);
    cout << "         " << flush;
    set_cursor(START_STATE, FRAME_POS_INFO_Y + 1);
    helper_board_state(
        &info->status, []() { cout << " You won!" << flush; },
        []() { cout << "You died!" << flush; });
    reset_bg();
}

void print_cell(const Board& board, const cell_info_t* const info) {
    if (board.is_cell_info_cursor(info)) {
        set_inverse_fg_bg();
    }

    if (info->is_flag && info->is_bomb && board.get_info().status.show_bomb) {
        print_cell(color_open_bg, 'C');
    } else if (info->is_flag) {
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
    cout << flush;
    if (board.is_cell_info_cursor(info)) {
        unset_inverse_fg_bg();
    }
}

void print_edge(unsigned int size) {
    while (size-- > 0) {
        cout << "   " << flush;
    }
}

void print_frame_block(const board_info_t* const info) {
    unsigned int size_index_y =
        info->size.y + FRAME_SIZE_INFO_Y + FRAME_SIZE_INPUT_Y;
    set_cursor(1, 1);
    while (size_index_y--) {
        set_background(color_frame_bg);
        print_edge(((info->size.x + 2) < 15) ? 15 : info->size.x + 2);
        reset_bg();
        cout << endl;
    }
}

void print_instructions(const board_info_t* const info) {
    const string instructions[3] = {"<h,j,k,l> left,down,up,right",
                                    "<space> open | <f> flag", "<p> previous"};
    const unsigned int START_INPUT_Y =
        CELL_HEIGHT * info->size.y + FRAME_SIZE_INFO_Y + 1;
    set_background(color_frame_bg);
    for (unsigned int i = 0; i < 3; i++) {
        set_cursor(FRAME_POS_START_TEXT, START_INPUT_Y + CELL_HEIGHT * (2 + i));
        cout << instructions[i] << endl;
    }
    reset_bg();
}

void print_grid(const Board& board) {
    unsigned int index_y = 0;
    board_status_t status = board.get_info().status;
    helper_board_state(
        &status, []() { set_foreground(color_done_bg); },
        []() { set_foreground(color_dead_bg); });

    function<void(const cell_info_t* const)> func_x =
        [&board](const cell_info_t* const info) { print_cell(board, info); };
    function<void(const cell_info_t* const)> func_y =
        [&index_y](const cell_info_t* const) {
            const unsigned int START_BOARD = CELL_HEIGHT * 4;
            set_cursor(FRAME_POS_START_TEXT,
                       START_BOARD + CELL_HEIGHT * index_y++);
        };
    board.grid_iterater(func_x, func_y);
    set_foreground(color_info_fg);
}

void BoardPrinter::print_frame(const Board& board) {
    board_info_t info = board.get_info();
    set_foreground(color_info_fg);
    clear_screen();
    print_frame_block(&info);
    print_instructions(&info);
}

void BoardPrinter::print(const Board& board) {
    board_info_t info = board.get_info();
    print_info(&info);
    print_grid(board);
    set_cursor_input(&info);
}
