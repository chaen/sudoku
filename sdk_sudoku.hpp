#ifndef SDK_SUDOKU_HPP
#define SDK_SUDOKU_HPP

#include <iostream>
#include <vector>
#include <stdexcept> 

#include "sdk_item.hpp"
#include "sdk_layer.hpp"
#include "sdk_hypo.hpp"


using namespace std;



class sdk_sudoku {

private:
  vector<sdk_layer> _layers;
  vector<sdk_hypo> _hypothesis;

  int solve();

public:
  sdk_sudoku(string filename);

};

sdk_sudoku::sdk_sudoku(string filename){

  sdk_layer originalLayer(filename);
  sdk_layer firstLayer;
  firstLayer.copy(originalLayer);
  this->_layers.push_back(firstLayer);
  this->solve();
  
}


int sdk_sudoku::solve(){
  int solved = 0;
  int totalTrial = 0;

  while (!solved){
    try{
 			sdk_layer & curLayer = this->_layers.back();
			int changes = -1;
			int pass = 1;
			while (changes){
		 	 cout << "PASS " << pass++ << endl;
		 	 totalTrial++;
		   curLayer.display();
			 changes = curLayer.make_one_pass();	
 	 	}
  
  		solved = curLayer.isComplete();
  		if (!solved){
  		  cout << "I could not solve" << endl;
  	  	// make hypothesis and new layer
				sdk_hypo hypo = curLayer.propose_hypothesis();
				int choice = curLayer.items[hypo.x][hypo.y].possibles.back();
				cout << "I make a new hypo (" << hypo.x << " " << hypo.y << ") : " << choice << endl;
				cout << "size of possibles " << curLayer.items[hypo.x][hypo.y].possibles.size() << endl;
				curLayer.items[hypo.x][hypo.y].possibles.pop_back();
				hypo.tried.push_back(choice);
				this->_hypothesis.push_back(hypo);
				sdk_layer newLayer;
				newLayer.copy(curLayer);
				newLayer.items[hypo.x][hypo.y] = choice;
			  newLayer.items[hypo.x][hypo.y].depth=this->_layers.size()+1;
				this->_layers.push_back(newLayer);  	  
  		}
  	}  catch (std::logic_error & e){
    	cout << "I catch " << e.what() << endl;
    	int stopStepingBack = 0;
    	while(!stopStepingBack){
    		// We remove the last layer
    		this->_layers.pop_back();
    		cout << "AMOUNT OF HYPO " << this->_hypothesis.size() << endl;
    		
    		sdk_hypo  & hypo = this->_hypothesis.back();
    		cout << "Last hypo on " << hypo.x << " " << hypo.y << endl;
    		cout << "\t Tried ";
    		for (vector<int>::iterator it = hypo.tried.begin(); it != hypo.tried.end(); it ++ ){
    		  cout << *it << " ";
    		}
    		cout << endl;
    		sdk_layer & otherLayer = this->_layers.back();
    	  if (otherLayer.items[hypo.x][hypo.y].possibles.size()){
    	    cout << "There are still possibilites ";
    	    for (vector<int>::iterator it = otherLayer.items[hypo.x][hypo.y].possibles.begin(); it != otherLayer.items[hypo.x][hypo.y].possibles.end(); it ++ ){
    		    cout << *it << " ";
    		  }
    		  cout << endl;
				  int choice = otherLayer.items[hypo.x][hypo.y].possibles.back();
				  cout << "I will try with " << choice << endl;
				  otherLayer.items[hypo.x][hypo.y].possibles.pop_back();
				  this->_hypothesis.back().tried.push_back(choice);
				  sdk_layer newLayer;
				  newLayer.copy(otherLayer);
			  	newLayer.items[hypo.x][hypo.y] = choice;
			  	newLayer.items[hypo.x][hypo.y].depth=this->_layers.size()+1;
			  	this->_layers.push_back(newLayer);  	
			  	stopStepingBack = 1;  
    	  } else {
    	    cout << "We exhausted all the choices" << endl;
    	    this->_hypothesis.pop_back();
    	  }
    	}
    	
  	}
  }

  cout << "FINALY in " << totalTrial << " attempts" << endl;
  this->_layers.back().display();
  
  for (int i = 0; i < this->_layers.size() + 1; i++){
    cout << "\t\033["<< 31 + i << "m"
           <<  "Layer" << i << " \033[0m" << endl;
  }

  return 0;
}

#endif
