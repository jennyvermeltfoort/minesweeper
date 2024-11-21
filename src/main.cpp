
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
    int robot_iterations;
    std::string output_file;
} arg_t;

arg_t parse_args(int argc, const char* argv[]) {
    arg_t args = {};
    static unordered_map<string, int*> map_args = {
        {"-w", &args.width},
        {"--width", &args.width},
        {"-h", &args.height},
        {"--height", &args.height},
        {"-b", &args.bomb_count},
        {"--bomb_count", &args.bomb_count},
        {"-i", &args.robot_iterations},
        {"--iterations", &args.robot_iterations},
    };
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
    BoardHandler input_handler = BoardHandler();
    arg_t args = parse_args(argc, argv);
    if (args.width < 1 || args.height < 1 || args.bomb_count < 1) {
        cout << "Usage: [-w,--width] <num> [-h,--height] <num> "
                "[-b,--bomb_count] <num> [-r,--robot] <output_path> [-i,--i] "
                "<num>"
             << endl;
        return 1;
    }
    if (args.robot_iterations > 0) {
        fstream output_file(args.output_file, fstream::out);
        if (!output_file.is_open()) {
            cout << "Unable to open file! file: " << args.output_file << endl;
            return 1;
        }
        input_handler.automated(args.width, args.height, args.bomb_count,
                                output_file, args.robot_iterations);
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
}
