//
// Created by sarah on 18/12/2019.
//

#ifndef FLIGHTSIMSHARED__RLARROW_H_
#define FLIGHTSIMSHARED__RLARROW_H_

#include "Command.h"
#include <map>
#include <string>

using namespace std;

class RLarrow : public Command {
  map<string, string>* _pathToVar;
 public:
  RLarrow(map<string, string>* pathToVar);
  virtual int execute(vector<string>& arguments, unsigned int index);
};

#endif //FLIGHTSIMSHARED__RLARROW_H_