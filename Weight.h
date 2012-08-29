#ifndef WEIGHT_H
#define WEIGHT_H

class Weight {
 public:
  virtual int weight(int i) = 0;
  virtual int weight(int i, int j) = 0;
};

#endif // WEIGHT_H
