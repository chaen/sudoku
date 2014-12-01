#ifndef SDK_ITEM_HPP
#define SDK_ITEM_HPP



/*!
 * \file sdk_item.hpp
 * \brief Item of a sudoku grid
 * \author Christophe Haen
 * \version 0.1
 */
 
 
#include <vector>
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf

using namespace std;


/*! \class sdk_item
 * \brief Class representing one box of the sudoku grid
 *
 *  It has a value between 1 and GRID_MAX
 */
class sdk_item {
public:
  vector<int> possibles; /*! When several possibilities */
  int value; /*! Value in the box (if set, implies that there are no other possibilities */
  int depth; /*! Just for display. At which hypothesis-level we are. */
  
  /*!
	 *  \brief Default constructor
	 *
	 *  Constructor
	 *
	 *  \param val : value of the box (default 0)
	 */
  sdk_item(int val = 0);
  
  /*!
  *  \brief Assignment operator
  *
  *  Sets the value of the box
  *
  * \param val : value of the box
  */
  sdk_item & operator=(const int val);
  
  /*!
  *  \brief Comparison operator between sdk_item and int
  *
  *  Allows to test sdk_item1 == integer value. Compares their values
  *
  *  \param it1 : first item
  *  \param val : integer value
  *  \returns bool : True if it1.value == val;
  */
  friend bool operator== (const sdk_item &it1, const int &val);

  /*!
  *  \brief Not operator
  *
  *  Check if the value is not null
  *
  *  \param it : sdk_item
  *  \returns bool True if it.value is not 0
  */
  friend bool operator!(const sdk_item &it);
};

sdk_item::sdk_item( int val){
	this->depth = 0;
  this->value = val;
}

sdk_item & sdk_item::operator=(const int val){
  this->value = val;
  return *this;
}


bool operator== (const sdk_item &it1, const int &val){
  return it1.value == val;
}

bool operator!(const sdk_item &it){
  return !it.value;
}
#endif
