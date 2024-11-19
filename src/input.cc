#include <cassert>
#include <functional>
#include <iostream>
#include <unordered_map>

#include "input.hh"

typedef struct CALLBACK_DATA_T {
    std::function<void(Board*)> callback_void;
    std::function<void(Board*, board_state_e)> callback_state;
    std::function<void(Board*, BoardStack*)> callback_stack;
    bool needs_store = false;
    board_state_e board_state;
} callback_data_t;

void board_store(Board* board, BoardStack* stack) {
    BoardEncoded* encoder = new BoardEncoded(board);
    stack->push(encoder);
}

void board_load_prev(Board* board, BoardStack* stack) {
    if (!stack->is_empty()) {
        BoardEncoded* encoder = stack->pop();
        encoder->decode(board);
    }
}

static std::unordered_map<char, callback_data_t> map_callback = {
    {'h', {.callback_void = &Board::cursor_move_west}},
    {'j', {.callback_void = &Board::cursor_move_south}},
    {'k', {.callback_void = &Board::cursor_move_north}},
    {'l', {.callback_void = &Board::cursor_move_east}},
    {'s',
     {.callback_state = &Board::toggle_state,
      .board_state = BOARD_STATE_SHOW_BOMB}},
    {' ',
     {.callback_void = &Board::cursor_set_open, .needs_store = true}},
    {'f',
     {.callback_void = &Board::cursor_set_flag, .needs_store = true}},
    {'p', {.callback_stack = board_load_prev}},
};

void BoardHandler::parse_input(const char c, bool* const is_end) {
    if (c == 'e') {
        *is_end = true;
        return;
    }

    if (map_callback.find(c) != map_callback.end()) {
        callback_data_t data = map_callback[c];

        if (data.needs_store &&
            (!board->is_state(BOARD_STATE_DEAD) &&
             !board->is_state(BOARD_STATE_DONE))) {
            board_store(board, stack);
        }

        if (data.callback_void) {
            data.callback_void(board);
        } else if (data.callback_state) {
            data.callback_state(board, data.board_state);
        } else if (data.callback_stack) {
            data.callback_stack(board, stack);
        }
    }

    printer->print();
}

BoardHandler::BoardHandler(Board* const _board) : board(_board) {
    stack = new BoardStack;
    printer = new BoardPrinter(board);
}

BoardHandler::~BoardHandler(void) {
    delete stack;
    delete printer;
}
