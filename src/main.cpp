

#include <fstream>
#include <iostream>
#include <iomanip>


int print_help(void)
{
     std::cout << "Help:\t-c <file_path_in> <file_path_out> for encode." << std::endl;
     std::cout << "\t-d <file_path_in> <file_path_out> for decode." << std::endl;
     return 1;
}

int main (int argc, char *argv[]) {
     std::fstream fs_in;
     std::ofstream fs_out;
     char c_prev = 0;
     char c_now = 0;
     int counter = 0;
     int fs_in_char_count = 1;
     int fs_out_char_count = 0;

     if (argc == 1)
     {
          return print_help();
     }
     else
     {
          switch(argv[1][1]) {
          case 'c':
               std::cout << "c";
               fs_in.open(argv[2]);
               fs_out.open(argv[3]);
               break;
          case 'd':
               std::cout << argv[3];
               fs_in.open(argv[2]);
               fs_out.open(argv[3]);
               break;
          default:
               return print_help();
          }
     }


     c_prev = fs_in.get();
     c_now = c_prev - 1;

     do {
          c_now = fs_in.get();
          fs_in_char_count++;
          if (c_now == c_prev)
          {
               counter++;
          }
          else 
          {
               if (c_prev == '\\')
               {
                    fs_out << '\\';
                    fs_out_char_count++;
               }
               else if (((c_prev >> 4) == 0x3) && ((c_prev & 0xF) <= 9))
               {
                    fs_out << '\\';
                    fs_out_char_count++;
               }
               fs_out << c_prev;
               fs_out_char_count++;
               c_prev = c_now;
               if (counter > 0)
               {
                    fs_out << counter + 1;
                    fs_out_char_count++;
                    counter = 0;
               }
               
          }
     } while (!fs_in.eof());

     std::cout << "Conversion rate: ";
     std::cout << std::fixed << std::setprecision(2) << static_cast<float>(fs_out_char_count) / static_cast<float>(fs_in_char_count) * 100.0 << std::endl;

     fs_in.close();
     fs_out.close();

     return 0;
}