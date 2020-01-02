#ifndef CONNECT_COMMAND_H
#define CONNECT_COMMAND_H
#include "Command.h"

using namespace std;

class ConnectControlClient : public Command {
  int _client_fd;
  int _port;
  map<string, double>* _symbolTable;

  void startClient(const char* dst_addr);

 public:

  /*
  * function that starts the connection between the simulator and program
  * as clients.
  */
  ConnectControlClient(map<string, double>* symbolTable);

  /*
  * do Command reads the arguments and sets the connection
  * between the simulator.
  * */
  virtual int execute(vector<string>& arguments, unsigned int index);
  void sendMessage(const string message);
  ~ConnectControlClient();
};

#endif //	!CONNECT_COMMAND_H