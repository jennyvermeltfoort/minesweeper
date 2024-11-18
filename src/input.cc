#include <functional>
#include <unordered_map>

#include "input.hh"

BoardInputHandler::BoardInputHandler(Board* _board)
    : board(_board), is_end(false) {}

void BoardInputHandler::parse_input(char c, bool* _is_end) {
    std::unordered_map<char, std::function<void(Board*)>>
        map_callback_void{
            {'h', &Board::cursor_move_west},
            {'j', &Board::cursor_move_south},
            {'k', &Board::cursor_move_north},
            {'l', &Board::cursor_move_east},
            {'s', &Board::toggle_show_bomb},
        };
    std::unordered_map<char, std::function<board_return_t(Board*)>>
        map_callback_ret{
            {' ', &Board::cursor_set_open},
            {'f', &Board::cursor_set_flag},
        };
    if (c == 'e') {
        is_end = true;
    } else if (map_callback_void.find(c) != map_callback_void.end()) {
        map_callback_void[c](board);
    } else if (map_callback_ret.find(c) != map_callback_ret.end()) {
        if (map_callback_ret[c](board) & BOARD_RETURN_STOP) {
            is_end = true;
        }
    }

    board->print();
    *_is_end = is_end;
}
