#include "stack.hpp"

BoardEncoded::BoardEncoded(const Board &board) : info(board.get_info()) {
    std::function<void(const cell_info_t *const)> func =
        [this](const cell_info_t *const info) { this->list->push_back(*info); };
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
            cell_info_t i = this->list->front();
            this->list->pop_front();
            *cell_info = i;
        };
    board.grid_iterater(func, nullptr);
    delete this;
}

BoardEncoded::~BoardEncoded(void) {
    list->clear();
    delete list;
}

void BoardStack::push(BoardEncoded *encoded) { list->push_back(encoded); }

BoardEncoded *BoardStack::pop(void) {
    BoardEncoded *encoded = list->front();
    list->pop_front();
    return encoded;
}

bool BoardStack::is_empty(void) { return list->empty(); }

BoardStack::~BoardStack(void) {
    while (!is_empty()) {
        BoardEncoded *encoded = pop();
        delete encoded;
    }
}
