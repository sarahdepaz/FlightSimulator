//
// Created by sarah on 18/12/2019.
//

#ifndef FLIGHTSIMSHARED__LRARROW_H_
#define FLIGHTSIMSHARED__LRARROW_H_

#include "Command.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class LRarrow : public Command {
  map<string, string> *_varToPath;
 public:
  LRarrow(map<string, string> *varToPath);
  virtual int execute(vector<string> &arguments, unsigned int index);
};

#endif //FLIGHTSIMSHARED__LRARROW_H_