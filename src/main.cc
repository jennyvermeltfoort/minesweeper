/* Build with: g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0.
 *
 * The application formats c or c++ source files, removing // prefixed
 * comments and rearranging identation. It verifies whether the input
 * file has a valid accolade balance. Furthermore a by the user
 * provided combination of letters is counted within the output file.
 * Numbers within the input file are determined to be Lychrel numbers
 * using the Lychrel algoritme.
 *
 * Author: Jenny Vermeltfoort (j.vermeltfoort@umail.leidenuniv.nl)
 * Date: 9-27-2024
 */

#include <fstream>
#include <iostream>

const uint8_t LETTERS_SIZE = 3;
const uint8_t EXPECTED_QUANTITY_ARGUMENTS = 6;
const uint8_t MAX_TAB_SIZE = 8;
const uint8_t ANSCII_NUMBER_MASK = 0XF;
const uint8_t ANSCII_NUMBER_0 = 48;
const uint8_t ANSCII_NUMBER_9 = 57;

typedef enum ERRNO {
    ERRNO_OK = 0,
    ERRNO_ERR,
} errno_e;

typedef enum ARG_POS {
    ARG_POS_INPUT = 1,
    ARG_POS_OUTPUT = 3,
    ARG_POS_LETTER = 5,
    ARG_POS_TAB = 7,
    ARG_POS_NONE = 9,
} arg_pos_e;

typedef enum LYCHREL_NUMBER {
    LYCHREL_NUMBER_NONE = 0,
    LYCHREL_NUMBER_OK,
    LYCHREL_NUMBER_DELAYED,
} lychrel_number_e;

typedef char letter_buf_t[LETTERS_SIZE];

/* Consume till logic, consumes all characters in file till
 * eof or until the function callback returns false. */
typedef bool (*consume_logic_t)(char c);
char fs_consume_till_logic(std::fstream &fs, consume_logic_t logic) {
    char c = 0;
    do {
        c = fs.get();
    } while (!fs.eof() && logic(c));
    return c;
}
inline bool anscii_is_int(char c) {
    return (c >= ANSCII_NUMBER_0 && c <= ANSCII_NUMBER_9);
}
inline bool anscii_is_whitespace(char c) {
    return (c == ' ' || c == '\t');
}
inline bool anscii_is_not_newline(char c) { return (c != '\n'); }

inline void cli_print_help(void) {
    std::cout << "-i <path> //path for input file." << std::endl;
    std::cout << "-o <path> //path for output file." << std::endl;
    std::cout << "-l <string> //search for letters in input file, "
                 "exactly 3 letters, no spaces, "
                 "only letters."
              << std::endl;
    std::cout << "-t <int> //lenght of tabs. Max length = 8"
              << std::endl;
    std::cout << "main.o -i <path> -o <path> -l <string> -t <int>."
              << std::endl;
};

inline errno_e parser_open_file_stream(std::fstream &fs, char *cbuf,
                                       std::ios_base::openmode mode) {
    fs.open(cbuf, mode);

    if (!fs.is_open()) {
        std::cout
            << "Failed to open file, please validate the path. File: "
            << cbuf << std::endl;
        return ERRNO_ERR;
    }

    return ERRNO_OK;
};

errno_e parser_letters(char *cbuf, letter_buf_t out) {
    uint8_t i = 0;

    do {
        if (i >= LETTERS_SIZE) {
            std::cout << "Too many letters provided." << std::endl;
            return ERRNO_ERR;
        }
        out[i] = cbuf[i] | 0x20;
        if (out[i] < 97 || out[i] > 122) {
            std::cout
                << "Only letters may be provided. Provided was: "
                << out[i] << std::endl;
            return ERRNO_ERR;
        }

        i++;
    } while (cbuf[i] != '\0');

    if (i != LETTERS_SIZE) {
        std::cout << "Not enough letters provided." << std::endl;
    }

    return ERRNO_OK;
}

/* Decide whether the number is a Lychrel number using the Lychrel
 * algoritme. A number is considered a Lychrel number when sum of the
 * number and its reverse exceed UINT32_MAX.
 */
lychrel_number_e lychrel_is_number(uint32_t number,
                                   uint8_t &iteration) {
    uint32_t storage = number;
    uint32_t reverse = 0;

    do {
        reverse = reverse * 10 + (storage % 10);
        storage /= 10;
    } while (storage != 0);

    if (reverse == number && iteration == 0) {
        return LYCHREL_NUMBER_OK;
    } else if (reverse == number) {
        return LYCHREL_NUMBER_DELAYED;
    } else if ((UINT32_MAX - number) < reverse) {
        return LYCHREL_NUMBER_NONE;
    }

    iteration++;
    return lychrel_is_number(number + reverse, iteration);
}

/* Removes comments and white lines. Reproduces indentation.
 */
errno_e fs_format(std::fstream &in, std::fstream &out,
                  uint8_t tab_size) {
    char c = in.get();
    char p = '\n';
    int16_t indent = 0;
    uint8_t i = 0;

    while (!in.eof()) {
        if (p == '\n' && (c == ' ' || c == '\t')) {
            c = fs_consume_till_logic(in, anscii_is_whitespace);
        }

        if (c == '/' && in.peek() == '/') {
            c = fs_consume_till_logic(in, anscii_is_not_newline);
        }

        if (c == '}') {
            indent--;
        }
        if (p == '\n' && c != '\n') {
            for (i = 0; i < (tab_size * ((indent < 0) ? 0 : indent));
                 i++) {
                out.put(' ');
            }
        }
        if (c == '{') {
            indent++;
        }

        // remove white lines.
        if (!(c == '\n' && p == '\n')) {
            out.put(c);
        }

        p = c;
        c = in.get();
    }

    if (indent != 0) {
        std::cout
            << "> An accolade inbalance was found: '" << indent
            << "' instance(s) of unresolved accolades. Positive "
               "means too many open accolades, negative means "
               "too many closed accolades."
            << std::endl;
        return ERRNO_ERR;
    }

    return ERRNO_OK;
}

/* Counts an exact cbuf combination of letters within the filestream.
 */
uint16_t fs_count_letters(std::fstream &fs, letter_buf_t cbuf) {
    uint16_t counter = 0;

    do {
        if (fs.get() == cbuf[0] && fs.get() == cbuf[1] &&
            fs.get() == cbuf[2]) {
            counter++;
        }
    } while (!fs.eof());

    return counter;
}

/* Parses all numbers in a filestream and verifies whether they are
 * lychrel numbers. Prints the results in the CLI. Numbers in the file
 * can't exceed UINT32_MAX.
 */
void fs_find_lychrel(std::fstream &fs) {
    lychrel_number_e rt_val = LYCHREL_NUMBER_NONE;
    uint32_t number = 0;
    uint8_t iteration = 0;
    char c = 0;

    do {
        c = fs.get();
        if (anscii_is_int(c) && (number * 10) < number) {
            std::cout << "Skipped the number starting with '"
                      << +number << "', it is too big to parse."
                      << std::endl;
            c = fs_consume_till_logic(fs, anscii_is_int);
            number = 0;
        }

        if (anscii_is_int(c)) {
            number = number * 10 +
                     static_cast<uint8_t>(c & ANSCII_NUMBER_MASK);
        } else if (number != 0) {
            rt_val = lychrel_is_number(number, iteration);
            if (rt_val == LYCHREL_NUMBER_OK) {
                std::cout << "Number: '" << +number
                          << "' is a palindroom, thus it is not a "
                             "lychrel number."
                          << std::endl;
            } else if (rt_val == LYCHREL_NUMBER_DELAYED) {
                std::cout
                    << "Number: '" << +number
                    << "' has a delayed palindroom found after '"
                    << +iteration
                    << "' iterations of the lychrel "
                       "algoritm, thus it is not a "
                       "lychrel number."
                    << std::endl;
            } else {
                std::cout
                    << "Can't find palindroom for base number: '"
                    << +number << "'  after '" << +iteration
                    << "' iterations of the lychrel "
                       "algoritm. Number thus could be considered "
                       "a lychrel number."
                    << std::endl;
            }
            iteration = number = 0;
        }
    } while (!fs.eof());
}

inline void fs_to_start(std::fstream &fs) {
    fs.clear();
    fs.flush();
    fs.sync();
    fs.seekg(0);
}

int main(int argc, char *argv[]) {
    errno_e rt_val = ERRNO_OK;
    std::fstream fs_input_file;
    std::fstream fs_output_file;
    letter_buf_t input_letters = {0};
    uint8_t tab_size = 0;

    if (argc < ARG_POS_NONE || argv[ARG_POS_INPUT][1] != 'i' ||
        argv[ARG_POS_OUTPUT][1] != 'o' ||
        argv[ARG_POS_LETTER][1] != 'l' ||
        argv[ARG_POS_TAB][1] != 't') {
        cli_print_help();
        return ERRNO_ERR;
    }
    if (parser_open_file_stream(fs_input_file,
                                argv[ARG_POS_INPUT + 1],
                                std::fstream::in) != ERRNO_OK) {
        return ERRNO_ERR;
    }
    if (parser_open_file_stream(fs_output_file,
                                argv[ARG_POS_OUTPUT + 1],
                                std::fstream::out) != ERRNO_OK) {
        return ERRNO_ERR;
    }
    if (parser_letters(argv[ARG_POS_LETTER + 1], input_letters) !=
        ERRNO_OK) {
        return ERRNO_ERR;
    }
    tab_size = static_cast<uint8_t>(argv[ARG_POS_TAB + 1][0] &
                                    ANSCII_NUMBER_MASK);
    if (tab_size > MAX_TAB_SIZE || argv[ARG_POS_TAB + 1][1] != '\0') {
        std::cout << "Tab size too large! Max is 8." << std::endl;
        return ERRNO_ERR;
    }

    rt_val = fs_format(fs_input_file, fs_output_file, tab_size);
    if (rt_val != ERRNO_OK) {
        return ERRNO_ERR;
    }

    fs_to_start(fs_input_file);
    fs_output_file.close();
    if (parser_open_file_stream(fs_output_file,
                                argv[ARG_POS_OUTPUT + 1],
                                std::fstream::in) != ERRNO_OK) {
        return ERRNO_ERR;
    }

    std::cout << "> Searching the letter combination: '"
              << input_letters << "'." << std::endl;
    std::cout << "Exact letter combination '" << input_letters
              << "' was found '"
              << +fs_count_letters(fs_input_file, input_letters)
              << "' time(s)." << std::endl;

    std::cout << std::endl;
    std::cout << "> Searching for lychrel numbers." << std::endl;
    fs_find_lychrel(fs_output_file);

    std::cout << std::endl;
    std::cout << "> File format done." << std::endl;

    fs_input_file.close();
    fs_output_file.close();

    return ERRNO_OK;
}