#ifndef SDK_ITEM_HPP
#define SDK_ITEM_HPP

#include <vector>
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream, std::stringbuf

using namespace std;

class sdk_item {
public:
  vector<int> possibles;
  int value;
  int depth;
  sdk_item(int val = 0);
  sdk_item & operator=(const int val);
  friend bool operator== (const sdk_item &it1, const int &val);
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
