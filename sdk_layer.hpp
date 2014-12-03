#ifndef SDK_LAYER_HPP
#define SDK_LAYER_HPP

/*!
 * \file sdk_layer.hpp
 * \brief Layer of a sudoku grid
 * \author Christophe Haen
 * \version 0.1
 */
 

#define GRID_SIZE 9

#include <vector>
#include <string>
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf
#include <fstream>
#include <algorithm>    // std::sort
#include <stdexcept> 

#include "sdk_item.hpp"
#include "sdk_hypo.hpp"

using namespace std;

vector<int> all_values;


/*! \class sdk_layer
 * \brief Class representing a layer the sudoku grid
 *
 *  A sudoku grid is a pile up of sdk_layer, one more layer per hypothesis
 *  we need to do
 */
class sdk_layer{

public:
  sdk_item items[GRID_SIZE][GRID_SIZE]; /*! grid of items */
  
   /*!
	 *  \brief Get the possible values at a position
	 *
	 *  Check the line, column and neighbourhood to check the possible values
	 *
	 *  \param x : line position
	 *  \param y : column position
	 *  \param list of possible values
	 */
  vector<int> getPossibles(int x, int y);
  
  /*!
	 *  \brief Default constructor
	 *
	 *  Constructor
	 *
	 */
  sdk_layer();
  
   /*!
	 *  \brief Initial constructor
	 *
	 *  Read from file.
	 *  The file has to be a 9 lines 9 columns (space separated)
	 *  description of the sudoku. 'x' is for unknown
	 *
	 *  \param filename : path of the file to read
	 */
  sdk_layer(string filename);
  
  /*!
	 *  \brief Copy a layer
	 *
	 *  Copy the values, reinitialise the possible, and increment the depth
	 *  of each item.
	 *  We don't make a copy constructor to do that because the copy constructor
	 *  is called when pushing back to a vector, and you don't want to do all this
	 *  when pushing back.
	 *
	 *  \param other : original layer
	 */
  sdk_layer & copy(const sdk_layer & other);

  /*!
	 *  \brief Go once through the layer and fill what can be filled
	 *
	 *  For each item, if it has one value, set it. If multiple, put or update them
	 *  throw std::logic_error if no solution to a item.
	 *
	 *  \returns amount of item changed
	 */
	int make_one_pass();
	
	/*!
	 *  \brief Checks if the layer is completed
	 *
	 *  \returns boolean
	*/  
	bool isComplete();
	
	/*!
	*  \brief pick one item on which to make hypothesis.
	*
	*  Choose an item with the minimum possibilities as base for the hypothesis.
	*
	*  Throws std::logic_error if no hypothesis possible
	*
	*  \returns sdk_hypo with the positions set.
	*/
	sdk_hypo propose_hypothesis();
	
	/*!
	* \brief Display in a bit cute way the layer
	*/
	void display();
	
	/*!
	*  \brief Operator to access the element with layer(x,y) 
	*/
	sdk_item& operator()(const int nCol, const int nRow);

};

sdk_layer::sdk_layer(){
}

sdk_layer::sdk_layer(string filename){
  ifstream myfile (filename.c_str());

  if (myfile.is_open())
  {
    for (int i = 0; i < GRID_SIZE; i++){
      cout << "Reading line " << i << endl;
      string line;
      getline(myfile, line);
      std::istringstream ss(line);
      vector<string> tokens;
      std::copy(istream_iterator<string>(ss),
        istream_iterator<string>(),
        back_inserter(tokens));
      cout << line << endl;
      for (int j = 0; j < GRID_SIZE; j++){
        int val = 0;
        try{
          val = stoi(tokens[j]);
        } catch  (...) {
        }
        this->items[i][j] = val;
      }
    }
    myfile.close();
  }

  else {
   throw std::runtime_error("Unable to open file");
  }
}

sdk_layer &  sdk_layer::copy(const sdk_layer & original){
  for (int i = 0; i < GRID_SIZE; i++){
    for (int j = 0; j < GRID_SIZE; j++){
    // Copy the item, clear the possibles, increase the depth of the item not set yet
      this->items[i][j] = original.items[i][j];
      this->items[i][j].possibles.clear();
      if (!this->items[i][j]){
        this->items[i][j].depth++;
      }
    }
  } 
  return *this;
}

vector<int> sdk_layer::getPossibles(int x, int y){
  // These vectors contain the values we find in a given line, column, neighbourhood. 
  vector<int> x_vec, y_vec, in_vec;
  for (int i = 0; i < GRID_SIZE; i ++){
    x_vec.push_back(this->items[x][i].value);
    y_vec.push_back(this->items[i][y].value);
  }
  
  // Start indexes of line and column of the neighbourhood.
  int start_x = ((int) x / 3) * 3;
  int start_y = ((int) y / 3) * 3;

  for (int i = start_x; i < start_x + 3; i ++){
    for (int j = start_y; j < start_y + 3; j ++){
      in_vec.push_back(this->items[i][j].value);
    } 
  }

  // Check the possible values w.r.t the line
  std::sort(x_vec.begin(), x_vec.end());
  vector<int> diff_x(GRID_SIZE);
  vector<int>::iterator diff_x_it;

  diff_x_it=set_difference (all_values.begin(), all_values.end(),
   												  x_vec.begin(), x_vec.end(), diff_x.begin());



  // Check the possible values w.r.t the column
  std::sort(y_vec.begin(), y_vec.end());
  vector<int> diff_y(GRID_SIZE);
  vector<int>::iterator diff_y_it;

  diff_y_it=set_difference (all_values.begin(), all_values.end(),
  												  y_vec.begin(), y_vec.end(), diff_y.begin());



  // Check the possible values w.r.t the neighbourhood
  std::sort(in_vec.begin(), in_vec.end());
  vector<int> diff_in(GRID_SIZE);
  vector<int>::iterator diff_in_it;

  diff_in_it=set_difference (all_values.begin(), all_values.end(), in_vec.begin(), in_vec.end(), diff_in.begin());

  // Check the possible values w.r.t the line AND the column
  vector<int> int_x_y(GRID_SIZE);
  vector<int>::iterator int_x_y_it;

  int_x_y_it = set_intersection(diff_x.begin(), diff_x.end(),
  														  diff_y.begin(), diff_y.end(), int_x_y.begin());
  

	
  // Check the possible values w.r.t the line AND the column AND the neighbourhood
  vector<int> int_in_x_y(GRID_SIZE);
  vector<int>::iterator int_in_x_y_it;

  int_in_x_y_it = set_intersection(int_x_y.begin(), int_x_y.end(), diff_in.begin(), diff_in.end(), int_in_x_y.begin());

	// Remove all the zeros in the possible
	int_in_x_y.erase(std::remove(int_in_x_y.begin(), int_in_x_y.end(), 0),
               int_in_x_y.end());

  return int_in_x_y;
}


int sdk_layer::make_one_pass(){
	int changes = 0;

  for (int i = 0; i < GRID_SIZE; i++){
    for (int j = 0; j < GRID_SIZE; j++){
      // If the value is not set yet
      if (!this->items[i][j]){
        // Check the possibles.
        vector<int> possibles = this->getPossibles(i,j);
        int amountOfPossibles = possibles.size();
        // There is no solution, throw logic_error
        if ( !amountOfPossibles){
        	throw std::logic_error( "No Solution" ); 
        } else if ( amountOfPossibles == 1) {
          // Only one possibilities ? It is our value !
        	this->items[i][j] = possibles[0];
        	this->items[i][j].possibles.clear();
        	changes++;
        } else {
          // Update the possibles if it has changed since last pass
        	if (this->items[i][j].possibles != possibles){
        	  this->items[i][j].possibles = possibles;	
        	  changes++;
        	}
        }
      }
    } 
  }
  
  cout << "In total " << changes << " changes" << endl;
  return changes;
}

bool sdk_layer::isComplete(){
  for (int i = 0; i < GRID_SIZE; i++){	
    for (int j = 0; j < GRID_SIZE; j++){
      if (! this->items[i][j])
        return false;
    }
  }
  return true;
}

void sdk_layer::display(){
  for (int i = 0; i < GRID_SIZE; i++){	
    if (!(i % 3)){
      cout << " ____________________" << endl;
    }
    for (int j = 0; j < GRID_SIZE; j++){
     if (!(j % 3)){
        cout << "|";
      }
      cout << "\033["<< 31 + this->items[i][j].depth << "m"
           <<  this->items[i][j].value << " \033[0m";
    }
    cout << "|" << endl;
  }
  cout << " ____________________" << endl;

}

sdk_hypo sdk_layer::propose_hypothesis(){
  cout << "make hypo " << GRID_SIZE <<  endl;
  int x = 0, y = 0, minLen = GRID_SIZE;
  sdk_hypo hypo;
  for (int i = 0; i < GRID_SIZE; i++){
    for (int j = 0; j < GRID_SIZE; j++){
      //cout << i << " " << j << " -> " << this->items[i][j].value << endl;
      int len = this->items[i][j].possibles.size();
      if (len && (len < minLen)){
      	minLen = len;
      	x = i;
      	y = j;
      }  
    }
  }
  
  if (minLen == GRID_SIZE)
    throw std::logic_error("No hypothesis possible");
    
  hypo.x = x;
  hypo.y = y; 
  cout << "I propose " << x << " " << y << "(";
	for (vector<int>::iterator it = this->items[x][y].possibles.begin(); it != this->items[x][y].possibles.end(); it ++ ){
		  cout << *it << " ";
		}
	cout << ")" << endl;
  return hypo;
}


sdk_item& sdk_layer::operator()(const int nCol, const int nRow){
	return this->items[nCol][nRow];
}



#endif
