#include <functional>
#include <unordered_map>

#include "input.hh"

void BoardInputHandler::cb_cursor_move_north(void) {
    board->cursor_move_north();
}
void BoardInputHandler::cb_cursor_move_south(void) {
    board->cursor_move_south();
}
void BoardInputHandler::cb_cursor_move_west(void) {
    board->cursor_move_west();
}
void BoardInputHandler::cb_cursor_move_east(void) {
    board->cursor_move_east();
}
void BoardInputHandler::cb_cursor_set_flag(void) {
    board->cursor_set_flag();
}
void BoardInputHandler::cb_cursor_set_open(void) {
    if (board->cursor_set_open() == BOARD_RETURN_STOP) {
        is_end = true;
    }
}
void BoardInputHandler::cb_toggle_show_bomb(void) {
    board->toggle_show_bomb();
}

BoardInputHandler::BoardInputHandler(Board* _board)
    : board(_board), is_end(false) {}

void BoardInputHandler::parse_input(char c, bool* _is_end) {
    std::unordered_map<char, std::function<void(BoardInputHandler*)>>
        map_callback{
            {'h', &BoardInputHandler::cb_cursor_move_west},
            {'j', &BoardInputHandler::cb_cursor_move_south},
            {'k', &BoardInputHandler::cb_cursor_move_north},
            {'l', &BoardInputHandler::cb_cursor_move_east},
            {'f', &BoardInputHandler::cb_cursor_set_flag},
            {' ', &BoardInputHandler::cb_cursor_set_open},
            {'s', &BoardInputHandler::cb_toggle_show_bomb},
        };
    if (c == 'e') {
        is_end = true;
    } else if (map_callback.find(c) != map_callback.end()) {
        map_callback[c](this);
    }

    board->print();
    *_is_end = is_end;
}
