#include "handler.hpp"

#include <cstdlib>
#include <ctime>
#include <functional>
#include <unordered_map>
using namespace std;

typedef struct CALLBACK_DATA_T {
    function<void(Board&)> callback_void;
    function<void(Board&, BoardStack&)> callback_stack;
    bool store = false;
} callback_data_t;

void board_store(const Board& board, BoardStack& stack) {
    BoardEncoded* encoder = new BoardEncoded(board);
    stack.push(encoder);
}

void board_load_prev(Board& board, BoardStack& stack) {
    if (!stack.is_empty()) {
        BoardEncoded* encoder = stack.pop();
        encoder->decode(board);
    }
}

bool BoardHandler::user_parse_input(Board& board, const char c) {
    static unordered_map<char, callback_data_t> map_callback = {
        {'h', {.callback_void = &Board::cursor_move_west}},
        {'j', {.callback_void = &Board::cursor_move_south}},
        {'k', {.callback_void = &Board::cursor_move_north}},
        {'l', {.callback_void = &Board::cursor_move_east}},
        {'s', {.callback_void = &Board::toggle_show_bomb}},
        {' ', {.callback_void = &Board::cursor_set_open, .store = true}},
        {'f', {.callback_void = &Board::cursor_set_flag, .store = true}},
        {'p', {.callback_stack = board_load_prev}},
    };
    if (c == 'e') {
        return true;
    }
    if (map_callback.find(c) != map_callback.end()) {
        callback_data_t data = map_callback[c];
        board_status_t status = board.get_info().status;
        if (data.store && !status.is_dead && !status.is_done) {
            board_store(board, stack);
        }
        if (data.callback_void) {
            data.callback_void(board);
        } else if (data.callback_stack) {
            data.callback_stack(board, stack);
        }
    }
    printer.print(board);
    return false;
}

void BoardHandler::user_init(const Board& board) {
    printer.print_frame(board);
    printer.print(board);
}

void auto_play_board(Board& board) {
    function<void(Board&)> directions[4] = {
        &Board::cursor_move_west, &Board::cursor_move_south,
        &Board::cursor_move_north, &Board::cursor_move_east};
    board_info_t info = board.get_info();
    while (!info.status.is_dead && !info.status.is_done) {
        for (unsigned int i = 0; i < 3; i++) {
            unsigned int direction = rand() % 4;
            unsigned int steps = rand() % (info.size.x / 2);
            while (steps-- > 0) {
                directions[(direction + i) % 4](board);
            }
        }
        board.cursor_set_open();
        info = board.get_info();
    }
}

void BoardHandler::automated(const unsigned int width,
                             const unsigned int height,
                             const unsigned int bomb_count,
                             fstream& output_file, unsigned int iterations) {
    srand(time(nullptr));
    Board board = Board(width, height, bomb_count);
    while (iterations-- > 0) {
        auto_play_board(board);
        board_info_t info = board.get_info();
        if (info.status.is_dead) {
            output_file << "lost " << +info.step_count << endl;
        } else {
            output_file << "won " << +info.step_count << endl;
        }
        board.reinitialize();
    }
}
