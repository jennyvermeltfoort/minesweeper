#include <iostream>
#include <fstream>

#define LETTERS_SIZE 3
#define EXPECTED_QUANTITY_ARGUMENTS 6
#define MAX_TAB_SIZE 8

typedef enum ERRNO {
    ERRNO_OK = 0,
    ERRNO_ERR,
    ERRNO_ERR_ACCOLADES,
} errno_e;

typedef enum ARG_POS {
    ARG_POS_INPUT = 1,
    ARG_POS_OUTPUT = 3,
    ARG_POS_LETTER = 5,
    ARG_POS_TAB = 7,
    ARG_POS_NONE = 9,
} arg_pos_e;

typedef char letter_buf_t[LETTERS_SIZE] ;

void cli_print_help(void){
    std::cout << "-i <path> //path for input file." << std::endl;
    std::cout << "-o <path> //path for output file." << std::endl;
    std::cout << "-l <string> //search for letters in input file, exactly 3 letters, no spaces, only letters." << std::endl;
    std::cout << "-t <int> //lenght of tabs. Max length = 8" << std::endl;
    std::cout << "main.o -i <path> -o <path> -l <string> -t <int>." << std::endl;
};

errno_e parser_open_file_stream(std::fstream &fs, char* cbuf, std::ios_base::openmode mode)
{
     fs.open(cbuf, mode);
    
     if (!fs.is_open())
     {
            std::cout << "Failed to open file, please validate the path. File: " << cbuf << std::endl;
            return ERRNO_ERR;
     }

     return ERRNO_OK;
};

errno_e parser_letters(char* cbuf, letter_buf_t out)
{
     uint8_t i = 0;

     do {
          if (i >= LETTERS_SIZE) {
               std::cout << "Too many letters provided." << std::endl;
               return ERRNO_ERR;
          }

          out[i] = cbuf[i] | 0x20;

          if (out[i] < 97 || out[i] > 122)
          {
               std::cout << "Only letters may be provided. Provided was: " << out[i] << std::endl;
               return ERRNO_ERR;
          }

          i++;
     } while (cbuf[i] != '\0');

     if (i != LETTERS_SIZE)
     {
          std::cout << "Not enough letters provided." << std::endl;
     }

     return ERRNO_OK;
}

char fs_consume_till_character(std::fstream &fs, char character)
{
    while (!fs.eof() && fs.get() != character);
    return character;
}

char fs_consume_tabs_and_spaces(std::fstream &fs)
{
    char c = 0;
    do {
        c = fs.get();
    } while (!fs.eof() && (c == ' ' || c == '\t'));

    return c;
}

errno_e fs_format(std::fstream &in, std::fstream &out, int tab_size)
{
    char c = in.get();
    char p = '\n';
    int16_t indent = 0;
    uint8_t i = 0;

    while (!in.eof()) {
        if (p == '\n') {
            if (c == ' ' || c == '\t')
            {
                c = fs_consume_tabs_and_spaces(in);
            }
        }
 
        if (c == '}') {
            indent--;
        }
        if (p == '\n' && c != '\n') {
            for (i = 0 ; i <  (tab_size * ((indent < 0) ? 0 : indent)) ; i++) { 
                out.put(' '); 
            }
        }
        if (c == '{') {
            indent++;
        }

        if (c == '/' && in.peek() == '/') {
            c = fs_consume_till_character(in, '\n');
        } 
        
        if (!(c == '\n' && p == '\n')) // remove white lines.
        {
            out.put(c);
        }

        p = c;
        c = in.get();
    }

    if (indent != 0)
    {
        std::cout << "Accolade offset found: " << indent << std::endl;
        return ERRNO_ERR_ACCOLADES;
    }

    return ERRNO_OK;
}

uint16_t fs_count_letters(std::fstream &fs, letter_buf_t cbuf)
{
    uint16_t counter = 0;
    do {
        if (fs.get() == cbuf[0] 
            && fs.get() == cbuf[1] 
            && fs.get() == cbuf[2]) {
            counter++;
        }
    } while (!fs.eof());

    return counter;
}

int main(int argc, char* argv[]) {
    errno_e rt_val = ERRNO_OK;
    std::fstream fs_input_file;
    std::fstream fs_output_file;
    letter_buf_t input_letters = {0};
    uint8_t tab_size = 0;
    uint16_t letter_counter = 0;

    if (argc < ARG_POS_NONE 
        || argv[ARG_POS_INPUT][1] != 'i'
        || argv[ARG_POS_OUTPUT][1] != 'o'
        || argv[ARG_POS_LETTER][1] != 'l'
        || argv[ARG_POS_TAB][1] != 't') {
            cli_print_help();
        return ERRNO_ERR;
    }
    if (parser_open_file_stream(fs_input_file, argv[ARG_POS_INPUT+1], std::fstream::in) != ERRNO_OK){
        return ERRNO_ERR;
    }
    if (parser_open_file_stream(fs_output_file, argv[ARG_POS_OUTPUT+1], std::fstream::in | std::fstream::out ) != ERRNO_OK){
        return ERRNO_ERR;
    }
    if (parser_letters(argv[ARG_POS_LETTER+1], input_letters) != ERRNO_OK) {
        return ERRNO_ERR;
    }
    tab_size = static_cast<uint8_t>(argv[ARG_POS_TAB+1][1]);
    if (tab_size > MAX_TAB_SIZE) {
        std::cout << "Tab size too large! Tab size: " << tab_size << std::endl;
        return ERRNO_ERR;
    }

    rt_val = fs_format(fs_input_file, fs_output_file, tab_size);
    if (rt_val == ERRNO_ERR_ACCOLADES)
    {
        std::cout << "Validation of accolades failed!" << std::endl;
    } else if (rt_val != ERRNO_OK) {
        return ERRNO_ERR;
    }

    fs_output_file.flush();
    fs_output_file.sync();
    fs_output_file.seekg(0);

    letter_counter = fs_count_letters(fs_output_file, input_letters);
    std::cout << "Letter combination " << input_letters << " was found " << letter_counter << " times."  << std::endl;
   
    fs_input_file.close();
    fs_output_file.close();

     return ERRNO_OK;
}