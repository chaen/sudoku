#ifndef SDK_LAYER_HPP
#define SDK_LAYER_HPP

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

class sdk_layer{
//  void readFile(string filename);  

public:
  sdk_item items[GRID_SIZE][GRID_SIZE];
  vector<int> getPossibles(int x, int y);  
  sdk_layer();
  sdk_layer(string filename);
  sdk_layer & copy(const sdk_layer & other);

	int make_one_pass();
	bool isComplete();
	
	sdk_hypo propose_hypothesis();
	
	void display();
	

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

  else cout << "Unable to open file";

}

sdk_layer &  sdk_layer::copy(const sdk_layer & original){
//   cout << "COPY LAYER !! " << endl;
  //this->items = original.items;
  for (int i = 0; i < GRID_SIZE; i++){
    for (int j = 0; j < GRID_SIZE; j++){
      this->items[i][j] = original.items[i][j];
      this->items[i][j].possibles.clear();
      if (!this->items[i][j]){
//         cout << i << " " << j << " ++ (" << this->items[i][j].depth << "-> " << this->items[i][j].depth + 1 << ")"  << endl;
        this->items[i][j].depth++;
      }
    }
  } 
  return *this;
}

vector<int> sdk_layer::getPossibles(int x, int y){
  vector<int> x_vec, y_vec, in_vec;
  for (int i = 0; i < GRID_SIZE; i ++){
    //cout << "push in x " << this->items[x][i].value << endl;
    //cout << "push in y " << this->items[i][y].value << endl;

    x_vec.push_back(this->items[x][i].value);
    y_vec.push_back(this->items[i][y].value);
  }
  int start_x = ((int) x / 3) * 3;
  int start_y = ((int) y / 3) * 3;

  //cout << "start_x " << start_x << " start_y " << start_y << endl;
  for (int i = start_x; i < start_x + 3; i ++){
    for (int j = start_y; j < start_y + 3; j ++){
      //cout << "push in in " << this->items[i][j].value << endl;
      in_vec.push_back(this->items[i][j].value);
    } 
  }

  std::sort(x_vec.begin(), x_vec.end());
  vector<int> diff_x(GRID_SIZE);
  vector<int>::iterator diff_x_it;

  diff_x_it=set_difference (all_values.begin(), all_values.end(), x_vec.begin(), x_vec.end(), diff_x.begin());


//   cout << "Diff with X vector ";
//   for (int i = 0; i < GRID_SIZE; i++){
//     cout << diff_x[i] << " ";
//   }
//   cout << endl;

  std::sort(y_vec.begin(), y_vec.end());
  vector<int> diff_y(GRID_SIZE);
  vector<int>::iterator diff_y_it;

  diff_y_it=set_difference (all_values.begin(), all_values.end(), y_vec.begin(), y_vec.end(), diff_y.begin());
//   cout << "Diff with Y vector ";
//   for (int i = 0; i < GRID_SIZE; i++){
//     cout << diff_y[i] << " ";
//   }
//   cout << endl;

  std::sort(in_vec.begin(), in_vec.end());
  vector<int> diff_in(GRID_SIZE);
  vector<int>::iterator diff_in_it;

  diff_in_it=set_difference (all_values.begin(), all_values.end(), in_vec.begin(), in_vec.end(), diff_in.begin());
//   cout << "Diff In vector ";
//   for (int i = 0; i < GRID_SIZE; i++){
//     cout << diff_in[i] << " ";
//   }
//   cout << endl;

  vector<int> int_x_y(GRID_SIZE);
  vector<int>::iterator int_x_y_it;

  int_x_y_it = set_intersection(diff_x.begin(), diff_x.end(), diff_y.begin(), diff_y.end(), int_x_y.begin());
  

  vector<int> int_in_x_y(GRID_SIZE);
  vector<int>::iterator int_in_x_y_it;

  int_in_x_y_it = set_intersection(int_x_y.begin(), int_x_y.end(), diff_in.begin(), diff_in.end(), int_in_x_y.begin());

	int_in_x_y.erase(std::remove(int_in_x_y.begin(), int_in_x_y.end(), 0),
               int_in_x_y.end());
  //int_in_x_y.resize(int_in_x_y_it - int_in_x_y.begin());

//   cout << "Possible ";
//   for (vector<int>::iterator i = int_in_x_y.begin(); i < int_in_x_y.end(); i++ ){
//     cout << *i << " " ;
//   }
//   cout << endl;

  return int_in_x_y;
}


int sdk_layer::make_one_pass(){
	int changes = 0;

  for (int i = 0; i < GRID_SIZE; i++){
    for (int j = 0; j < GRID_SIZE; j++){
      if (!this->items[i][j]){
//         cout << "unknown at " << i << " " << j << endl;
        vector<int> possibles = this->getPossibles(i,j);
        int amountOfPossibles = possibles.size();
        // There is no solution
        if ( !amountOfPossibles){
//         	cout << "No solution !!" << endl;
        	throw std::logic_error( "No Solution" ); 
        } else if ( amountOfPossibles == 1) {
//           cout << "Only " << possibles[0] << " is possible" << endl;
        	this->items[i][j] = possibles[0];
        	this->items[i][j].possibles.clear();
        } else {
//           cout << "Several options ";
//           for (vector<int>::iterator it = possibles.begin(); it != possibles.end(); it ++){
//             cout << *it << " ";
//           }
//           cout << endl;
        	if (this->items[i][j].possibles != possibles){
//         	  cout << "Update from previous (";
//         	  for (vector<int>::iterator it = this->items[i][j].possibles.begin(); it != this->items[i][j].possibles.end(); it ++){
//             cout << *it << " ";
//             }
//             cout << ")" << endl;
        	  changes++;
        	  this->items[i][j].possibles = possibles;	
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

//  cout << "\033[31mbold red//  text\033[0m" << endl;
//  cout << "\033[32mbold red text\033[0m" << endl;
//  cout << "\033[1mbold red text\033[0m" << endl;
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


#endif
