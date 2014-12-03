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


public:
  sdk_sudoku(string filename);
  int solve();

};

sdk_sudoku::sdk_sudoku(string filename){

  sdk_layer originalLayer(filename);
  sdk_layer firstLayer;
  firstLayer.copy(originalLayer);
  this->_layers.push_back(firstLayer);
  
}


int sdk_sudoku::solve(){

  int solved = 0;
  int totalTrial = 0;
	
	// Continue until we did not solve it
  while (!solved){
    try{
      // Check the upper most layer
 			sdk_layer & curLayer = this->_layers.back();
 			
			int changes = -1; 			// Amount of changes that was done on the last pass
			int pass = 1;						// Amount of passes done on this layer
			
			// As long as the pass produces some changes, we continue
			while (changes){
		 	 cout << "PASS " << pass++ << endl;
		 	 totalTrial++;
		   //curLayer.display();
			 changes = curLayer.make_one_pass();	
 	 	}
  		// Check if the layer is now complete
  		solved = curLayer.isComplete();
  		
  		// If it is not complete, we make an hypothesis on this layer, and create a new one
  		if (!solved){
  		  cout << "I could not solve" << endl;
  		  
  	  	// make hypothesis and new layer
				sdk_hypo hypo = curLayer.propose_hypothesis();
				// The choice we make is the last one of the possible (no reason for that :) )
				int choice = curLayer(hypo.x, hypo.y).possibles.back();
				
				cout << "I make a new hypo (" << hypo.x << " " << hypo.y << ") : " << choice << endl;
				cout << "size of possibles " << curLayer(hypo.x, hypo.y).possibles.size() << endl;
				// We remove it of the possibles of the item so we don't pick it up again later
				curLayer(hypo.x, hypo.y).possibles.pop_back();
				// We add it the the tried list of the hypotheses
				hypo.tried.push_back(choice);
				// and we push the hypothesis on the list
				this->_hypothesis.push_back(hypo);
				
				// We create the new layer as a copy of the current one
				sdk_layer newLayer;
				newLayer.copy(curLayer);
				// And set the hypothesis at its place
				newLayer(hypo.x, hypo.y) = choice;
			  newLayer(hypo.x, hypo.y).depth=this->_layers.size()+1;
				this->_layers.push_back(newLayer);  	  
  		}
  	}  catch (std::logic_error & e){
  			// logic_error exceptions can happen if  
  			//    1) There is no solution to an item of the layer
  			//		2) There is no hypothesis possible to be made
  			//
  			//	Open question : is 1 equivalent to 2 ?
  			//
  			//  In any case, when this happens, we remove the last layer, 
  			//  go back to the hypothesis we've made, and update it to
  			//  take the next choice. If no choice anymore, we loop
  			//  to the previous layer and so on until we can continue.
  			//
  			//  Open question : is the algo resilient to impossible sudoku at all ?
  			//     Probably vector fault because we will pop an empty layer vector
  	
    	bool stopStepingBack = false; 
    	// This is set to true once we found a layer where we can make a new hypothesis
    	while(!stopStepingBack){
    		// We remove the last layer since it was unsolvable
    		this->_layers.pop_back();   
    		
    		// We check the last hypothesis 		
    		sdk_hypo  & hypo = this->_hypothesis.back();
    		
    		// Display the hypo
    		cout << "Last hypo on " << hypo.x << " " << hypo.y << endl;
    		cout << "\t Tried ";
    		for (vector<int>::iterator it = hypo.tried.begin(); it != hypo.tried.end(); it ++ ){
    		  cout << *it << " ";
    		}
    		cout << endl;
    		
    		// Take the previous layer
    		sdk_layer & otherLayer = this->_layers.back();
    		// We check if there are still possibilities on the item we chose as a hypothesis
    	  if (otherLayer(hypo.x, hypo.y).possibles.size()){
    	  
    	    // We display the other possibilities
    	    cout << "There are still possibilites ";
    	    for (vector<int>::iterator it = otherLayer(hypo.x, hypo.y).possibles.begin();
    	                  it != otherLayer(hypo.x, hypo.y).possibles.end(); it ++ ){
    		    cout << *it << " ";
    		  }
    		  cout << endl;
    		  
    		  // Next choice 
				  int choice = otherLayer(hypo.x, hypo.y).possibles.back();
				  cout << "I will try with " << choice << endl;
				  // Remove it from the future possibles
				  otherLayer(hypo.x, hypo.y).possibles.pop_back();
				  // Add it to the tried 
				  this->_hypothesis.back().tried.push_back(choice);
				  
				  // Create a new layer based on this hypothesis
				  sdk_layer newLayer;
				  newLayer.copy(otherLayer);
			  	newLayer(hypo.x, hypo.y) = choice;
			  	newLayer(hypo.x, hypo.y).depth=this->_layers.size()+1;
			  	this->_layers.push_back(newLayer);
			  	
			  	// Okay, stop stepping back  	
			  	stopStepingBack = true;  
    	  } else {
    	    // There is no choice anymore on the hypothesis
    	    // So we remove it and keep looping
    	    cout << "We exhausted all the choices" << endl;
    	    this->_hypothesis.pop_back();
    	  }
    	}
    	
  	}
  }

	// We solved it. Display it, and the legend
  cout << "FINALY in " << totalTrial << " attempts" << endl;
  this->_layers.back().display();
  
  for (int i = 0; i < this->_layers.size() + 1; i++){
    cout << "\t\033["<< 31 + i << "m"
           <<  "Layer" << i << " \033[0m" << endl;
  }

  return 0;
}

#endif
