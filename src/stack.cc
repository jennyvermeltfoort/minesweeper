#include "stack.hh"

BoardEncoded::BoardEncoded(Board *board) {
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
    board->grid_iterater(func, nullptr);
}

void BoardEncoded::decode(Board *board) {
    const board_size_t size = board->get_info().size;
    cell_encoded_t *cell = start;

    if (info.size.x != size.x || info.size.y != size.y) {
        return;
    }

    board->set_status(info.status);
    std::function<void(cell_info_t *const)> func =
        [&cell](cell_info_t *const info) {
            info->state = cell->info.state;
            info->bomb_count = cell->info.bomb_count;
            cell = cell->next;
        };
    board->grid_iterater(func, nullptr);
    delete this;
}

BoardEncoded::~BoardEncoded(void) {
    cell_encoded_t *cell = start;
    while (cell != nullptr) {
        cell_encoded_t *prev = cell;
        cell = cell->next;
        delete prev;
    }
}

void BoardStack::push(BoardEncoded *encoded) {
    board_stack_cell_t *cell = new board_stack_cell_t;
    cell->encoded = encoded;
    cell->next = start;
    start = cell;
}

BoardEncoded *BoardStack::pop(void) {
    BoardEncoded *encoded = start->encoded;
    board_stack_cell_t *prev = start;
    start = start->next;
    delete prev;
    return encoded;
}

bool BoardStack::is_empty(void) { return (start == nullptr); }

BoardStack::~BoardStack(void) {
    while (!is_empty()) {
        BoardEncoded *encoded = pop();
        delete encoded;
    }
}
