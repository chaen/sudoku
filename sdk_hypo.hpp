#ifndef SDK_HYPO_HPP
#define SDK_HYPO_HPP

class sdk_hypo{
public:
  int x;
  int y;
  vector<int> tried;
  int value;

  sdk_hypo(int x = 0, int y = 0){
    this->x = x;
    this->y = y;
  }
};

#endif