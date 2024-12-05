#include "stack.hpp"

BoardEncoded::BoardEncoded(const Board &board) : info(board.get_info()) {
    std::function<void(const cell_info_t *const)> func =
        [this](const cell_info_t *const info) {
            this->llist->push_back(*info);
        };
    board.grid_iterater(func, nullptr);
}

void BoardEncoded::decode(Board &board) {
    const board_size_t size = board.get_info().size;
    if (info.size.x != size.x || info.size.y != size.y) {
        return;
    }
    board.set_status(info.status);
    std::function<void(const cell_info_t *const)> func =
        [this](const cell_info_t *const info) {
            cell_info_t *cell_info = const_cast<cell_info_t *>(info);
            cell_info_t i = this->llist->front();
            this->llist->pop_front();
            *cell_info = i;
        };
    board.grid_iterater(func, nullptr);
    delete this;
}

BoardEncoded::~BoardEncoded(void) {
    llist->clear();
    delete llist;
}

void BoardStack::push(BoardEncoded *encoded) { llist->push_back(encoded); }

BoardEncoded *BoardStack::pop(void) {
    BoardEncoded *encoded = llist->back();
    llist->pop_back();
    return encoded;
}

bool BoardStack::is_empty(void) { return llist->empty(); }

BoardStack::~BoardStack(void) {
    while (!is_empty()) {
        BoardEncoded *encoded = pop();
        delete encoded;
    }
}
