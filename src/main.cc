
#include <iostream>
#include <functional>
#include <unordered_map>

#include "stack.h"
#include "world.h"

class BoardInputHandler {
	private:
	Board *board;
	bool is_end;

	void cb_cursor_move_north(void) {
		board->cursor_move_north();
	}
	void cb_cursor_move_south(void) {
		board->cursor_move_south();
	}
	void cb_cursor_move_west(void) {
		board->cursor_move_west();
	}
	void cb_cursor_move_east(void) {
		board->cursor_move_east();
	}
	void cb_cursor_set_flag(void) {
		board->cursor_set_flag();
	}
	void cb_cursor_set_open(void) {
		if (board->cursor_set_open() == BOARD_RETURN_STOP) {
			is_end = true;
		}
	}
	void cb_toggle_show_bomb(void) {
		board->toggle_show_bomb();
	}

	public:
	BoardInputHandler(Board* _board) : board(_board), is_end(false) {}

		void parse_input(char c, bool* _is_end) {
    std::unordered_map<char, std::function<void(BoardInputHandler *)>>
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

	if (*_is_end == true) {
		std::cout << "Press enter to exit..." << std::endl;
		std::cin.get();
	}
		}

};


int main(void) {
    const unsigned int BOARD_SIZE_X = 20;
    const unsigned int BOARD_SIZE_y = 20;
    Board board(BOARD_SIZE_X, BOARD_SIZE_y, 1);
    BoardInputHandler input_handler(&board);
    bool is_end = false;

    board.print();
    while(is_end == false) {
	char c = std::cin.get();
	input_handler.parse_input(c, &is_end);
    }
}
