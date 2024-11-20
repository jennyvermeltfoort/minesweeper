#include "handler.hpp"

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

void BoardHandler::parse_input(const char c, bool* const is_end) {
    if (c == 'e') {
        *is_end = true;
        return;
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

    printer.print();
}

BoardHandler::BoardHandler(Board& _board)
    : board(_board),
      stack(BoardStack()),
      printer(BoardPrinter(_board)),
      printer_enabled(false) {}
