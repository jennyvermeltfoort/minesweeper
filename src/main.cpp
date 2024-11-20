
#include <iostream>
#include <string>
#include <unordered_map>

#include "board.hpp"
#include "handler.hpp"
#include "print.hpp"
#include "stack.hpp"

typedef struct ARG_T {
    unsigned int width;
    unsigned int height;
    unsigned int bomb_count;
    unsigned int dry;
} arg_t;

arg_t parse_args(int argc, const char* argv[]) {
    arg_t args = {0};
    static std::unordered_map<std::string, unsigned int*> map_args = {
        {"-w", &args.width},      {"--width", &args.width},
        {"-h", &args.height},     {"--height", &args.height},
        {"-b", &args.bomb_count}, {"--bomb_count", &args.bomb_count},
        {"-d", &args.dry},        {"--dry", &args.dry},
    };

    while (argc > 1) {
        if (map_args.find(argv[argc - 2]) != map_args.end()) {
            unsigned int* ptr = map_args[argv[argc - 2]];
            *ptr = atoi(argv[argc - 1]);
        }
        argc -= 2;
    }

    return args;
}

int main(int argc, const char* argv[]) {
    arg_t args = parse_args(argc, argv);
    if (!args.width || !args.height || !args.bomb_count) {
        std::cout << "Usage: [-w,--width] <num> [-h,--height] <num> "
                     "[-b,--bomb_count] <num> [-d,--debug] <num>"
                  << std::endl;
        return 1;
    }
    Board board = Board(args.width, args.height, args.bomb_count);
    BoardHandler input_handler = BoardHandler(board);
    bool is_end = false;

    if (args.dry > 0) {
        return 1;
    }

    input_handler.print_frame();
    while (is_end == false) {
        char c = std::cin.get();
        input_handler.parse_input(c, &is_end);
    }
}
