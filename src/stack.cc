#include "stack.hh"

void BoardEncoder::encode(Board *board) {
    info = board->get_info();
    start = new cell_encoded_t;
    cell_encoded_t *cell = start;
    std::function<void(cell_info_t *const)> func =
        [&cell](cell_info_t *const info) {
            cell->info.state = info->state;
            cell->info.bomb_count = info->bomb_count;
            cell->next = new cell_encoded_t;
            cell = cell->next;
        };
    board->grid_iterater(func);
}

void BoardEncoder::decode(Board *board) {
    cell_encoded_t *cell = start;
    board->set_info(info);
    std::function<void(cell_info_t *const)> func =
        [&cell](cell_info_t *const info) {
            cell_encoded_t *prev = cell;
            info->state = cell->info.state;
            info->bomb_count = cell->info.bomb_count;
            cell = cell->next;
            delete prev;
        };
    board->grid_iterater(func);
    start = nullptr;
}

bool BoardEncoder::is_encoded(void) { return (start != nullptr); }

void BoardStack::push(BoardEncoder *encoded) {
    if (!encoded->is_encoded()) {
        return;
    }

    board_stack_cell_t *cell = new board_stack_cell_t;
    cell->encoded = encoded;
    cell->next = start;
    start = cell;
}

BoardEncoder *BoardStack::pop(void) {
    BoardEncoder *encoded = start->encoded;
    board_stack_cell_t *prev = start;
    start = start->next;
    delete prev;
    return encoded;
}

bool BoardStack::is_empty(void) { return (start == nullptr); }
