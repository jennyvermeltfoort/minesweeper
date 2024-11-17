
#include "board.hh"

#ifndef __GUARD_INPUT_H
#define __GUARD_INPUT_H

class BoardInputHandler {
   private:
    Board* board;
    bool is_end;

    void cb_cursor_move_north(void); 
    void cb_cursor_move_south(void);
    void cb_cursor_move_west(void);
    void cb_cursor_move_east(void);
    void cb_cursor_set_flag(void);
    void cb_cursor_set_open(void);
    void cb_toggle_show_bomb(void);

   public:
    BoardInputHandler(Board* _board);
    void parse_input(char c, bool* _is_end);
};

#endif  // __GUARD_INPUT_H
