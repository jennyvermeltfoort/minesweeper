/**
 * g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
 *
 * The game Minesweeper. The code is split up in modules:
 *  - handler -> interface between the user, the board and the stack.
 *  - board -> all logic to manage the game, one board is one game.
 *  - stack -> the stack which is capable of storing and retrieving board
 * states.
 *  - print -> cli printing of the board.
 *
 * Date: 07-12-2024
 * Author: Jenny Vermeltfoort
 * Number: 3787494
 */
#include <iostream>
#include <string>
#include <unordered_map>

#include "board.hpp"
#include "handler.hpp"
#include "print.hpp"
#include "stack.hpp"
using namespace std;

typedef struct ARG_T {
    int width;
    int height;
    int bomb_count;
    int iters;
    string output_file;
} arg_t;

arg_t parse_args(int argc, const char* argv[]) {
    arg_t args = {};
    static unordered_map<string, int*> map_args = {
        {"-w", &args.width},      {"--width", &args.width},
        {"-h", &args.height},     {"--height", &args.height},
        {"-b", &args.bomb_count}, {"--bombs", &args.bomb_count},
        {"-i", &args.iters},      {"--iter", &args.iters}};
    //   "-r", "--robot output file"
    while (argc > 1) {
        if (map_args.find(argv[argc - 2]) != map_args.end()) {
            int* ptr = map_args[argv[argc - 2]];
            *ptr = atoi(argv[argc - 1]);
        }
        if (argv[argc - 2][1] == 'r' || argv[argc - 2][2] == 'r') {
            args.output_file = argv[argc - 1];
        }
        argc -= 2;
    }
    return args;
}

int main(int argc, const char* argv[]) {
    BoardHandler input_handler;
    arg_t args = parse_args(argc, argv);
    if (args.width < 1 || args.height < 1 || args.bomb_count < 1) {
        cout << "Usage: minesweeper [-w,--width] <num> [-h,--height] <num> "
                "[-b,--bomb_count] <num> [-r,--robot] <output_path> "
                "[-i,--iters] <num>"
             << endl;
        return 1;
    }
    if (args.iters > 0) {
        fstream output_file(args.output_file, fstream::out);
        if (!output_file.is_open()) {
            cout << "Unable to open file! file: " << args.output_file << endl;
            return 1;
        }
        input_handler.automated(args.width, args.height, args.bomb_count,
                                output_file, args.iters);
        output_file.close();
        return 0;
    }
    Board board = Board(args.width, args.height, args.bomb_count);
    bool is_end = false;
    input_handler.user_init(board);
    while (is_end == false) {
        char c = cin.get();
        is_end = input_handler.user_parse_input(board, c);
    }
    return 0;
}
