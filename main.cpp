#include <string>
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf

#include "sdk_item.hpp"
#include "sdk_sudoku.hpp"

using namespace std;

int main(int argc, char ** argv){
  for (int i = 0; i < GRID_SIZE; i++)
    all_values.push_back(i+1);
    
  if (argc != 2){
    cout << "Usage : " << argv[0] << " <sudokuFile> " << endl;
    cout << "The file has to contain 9 lines. On each line, the 9 \n\
             values separated by space. 'x' is for unknown" << endl;
    return 1;
  }
  sdk_sudoku l(argv[1]);
  cout << "toujours la " << endl;
  return 0;

}  
