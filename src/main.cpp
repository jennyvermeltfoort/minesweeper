

#include <iostream>
#include <fstream>

#define LETTERS_SIZE 3
#define EXPECTED_QUANTITY_ARGUMENTS 6

typedef enum ERRNO {
    ERRNO_OK = 0,
    ERRNO_ERR,
} errno_t;

std::fstream fs_input_file;
std::fstream fs_output_file;
char input_letters[LETTERS_SIZE] = {0};

void cli_print_help(void){
     std::cout << "-i <path> //path for input file." << std::endl;
     std::cout << "-o <path> //path for output file." << std::endl;
     std::cout << "-l <letters> //search for letters in input file, exactly 3 letters, no spaces, only letters." << std::endl;
};

errno_t parser_open_file_stream(std::fstream *fs, char* buf)
{
     fs->open(buf);
     
     if (!fs->is_open())
     {
          std::cout << "Failed to open file, please validate the path. File: " << buf << std::endl;
               return ERRNO_ERR;
     }

     return ERRNO_OK;
};

errno_t parser_letters(char* buf)
{
     uint8_t i = 0;

     do {
          if (i >= LETTERS_SIZE) {
               std::cout << "Too many letters provided." << std::endl;
               return ERRNO_ERR;
          }

          input_letters[i] = buf[i] | 0x20;

          if (input_letters[i] < 97 || input_letters[i] > 122)
          {
               std::cout << "Only letters may be provided. Provided was: " << input_letters[i] << std::endl;
               return ERRNO_ERR;
          }

          i++;
     } while (buf[i] != '\0');

     if (i != LETTERS_SIZE)
     {
          std::cout << "Not enough letters provided." << std::endl;
     }

     return ERRNO_OK;
}

int main(int argc, char* argv[]) {
    uint8_t i = 0;
    uint8_t arg_init = 0X7;
    errno_t rt_val = ERRNO_OK;

    if (argc != (EXPECTED_QUANTITY_ARGUMENTS + 1)) {
          cli_print_help();
          std::cout << "Unexpected arguments, or not all required arguments provided. Args: " << argc << std::endl;
          return ERRNO_ERR;
     }

    for (i = 1; i < argc ; i+=2)
    {
          switch (argv[i][1])
          {
               case 'i':
                    rt_val = parser_open_file_stream(&fs_input_file, argv[i+1]);
                    arg_init &= false << 0;
                    break;
               case 'o':
                    rt_val = parser_open_file_stream(&fs_output_file, argv[i+1]);
                    arg_init &= false << 1;
                    break;
               case 'l':
                    rt_val = parser_letters(argv[i+1]);
                    arg_init &= false << 2;
                    break;
               default:
                    cli_print_help();
                    std::cout << "Invalid argument provided. Argument: " << argv[i] << std::endl;
                    return ERRNO_ERR;
          }

          if (rt_val != ERRNO_OK)
          {
               return rt_val;
          }
     }

     if (arg_init != 0){
          std::cout << "Invalid arguments provided, likely duplicates." << std::endl;
          return ERRNO_ERR;

     }
     

     return ERRNO_OK;
}