//
// Created by sarah on 02/01/2020.
//

#ifndef FLIGHT_NEW__LEXER_H_
#define FLIGHT_NEW__LEXER_H_

#include <string>
#include <vector>

using namespace std;
/**
 * Lexer class
*  Iseperate the xml to lines .
* */
class Lexer {
 public:
  static vector<string> lexer(string line);
};

#endif //FLIGHT_NEW__LEXER_H_
