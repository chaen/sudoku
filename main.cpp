/**
 * \file main.cpp
 * \brief Sudoku solver
 * \author Christophe Haen
 * \version 0.1
 * \date 30 Nov. 2014
 *
 * This solves sudoku, full stop :-)
 *
 */

#include <string>
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf
#include <stdexcept> 

#include "sdk_item.hpp"
#include "sdk_sudoku.hpp"

using namespace std;

int main(int argc, char ** argv){

	// Initialise once for all a vector that runs from 1 to 9
  for (int i = 0; i < GRID_SIZE; i++)
    all_values.push_back(i+1);
    
  if (argc != 2){
    cout << "Usage : " << argv[0] << " <sudokuFile> " << endl;
    cout << "The file has to contain 9 lines. On each line, the 9 \n\
             values separated by space. 'x' is for unknown" << endl;
    return 1;
  }
  
  try{
    sdk_sudoku sdk(argv[1]);
  
    sdk.solve();
  } catch (std::runtime_error & e){
    cout << e.what() << endl;
  }
  return 0;

}  
