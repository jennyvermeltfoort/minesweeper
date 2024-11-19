#include <functional>
#include <iostream>
#include <unordered_map>

#include "input.hh"

void board_store(Board* board, BoardStack* stack) {
    BoardEncoder* encoder = new BoardEncoder;
    encoder->encode(board);
    stack->push(encoder);
}

void board_load_prev(Board* board, BoardStack* stack) {
    if (stack->is_empty()) {
        return;
    }
    BoardEncoder* encoder = stack->pop();
    std::cout << +encoder->is_encoded() << std::endl;
    encoder->decode(board);
    delete encoder;
}

BoardInputHandler::BoardInputHandler(Board* _board) : board(_board) {
    stack = new BoardStack;
}
BoardInputHandler::~BoardInputHandler(void) { delete stack; }

void BoardInputHandler::parse_input(char c, bool* is_end) {
    static std::unordered_map<char, std::function<void(Board*)>>
        map_callback_void{
            {'h', &Board::cursor_move_west},
            {'j', &Board::cursor_move_south},
            {'k', &Board::cursor_move_north},
            {'l', &Board::cursor_move_east},
            {'s', &Board::toggle_show_bomb},
        };
    static std::unordered_map<char, std::function<void(Board*)>>
        map_callback_store{
            {' ', &Board::cursor_set_open},
            {'f', &Board::cursor_set_flag},
        };
    static std::unordered_map<
        char, std::function<void(Board*, BoardStack*)>>
        map_callback_stack{
            {'p', board_load_prev},
        };

    if (c == 'e') {
        *is_end = true;
        return;
    } else if (map_callback_void.find(c) != map_callback_void.end()) {
        map_callback_void[c](board);
    } else if (map_callback_store.find(c) !=
               map_callback_store.end()) {
        board_store(board, stack);
        map_callback_store[c](board);
    } else if (map_callback_stack.find(c) !=
               map_callback_stack.end()) {
        map_callback_stack[c](board, stack);
    }

    board->print();
}
