#include "DataReaderServer.h"
#include "StringHelper.h"
#include <algorithm>
#include <iostream>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <mutex>
#define MAX_PORT_SIZE 65536
#define MIN_PORT_SIZE 1
#define MIN_SPEED 1
#define SECOND_IN_MILLI 1000

using namespace std;

DataReaderServer::~DataReaderServer() {
  close(this->_sockID);
}

DataReaderServer::DataReaderServer(map<string,double>* symbolTable,
                                   map<string, string>* pathToVar,

                                   Modifier* modifier) {
  _symbolTable = symbolTable;
  _pathToVar = pathToVar;
  _modifier = modifier;
  _argumentsAmount = 1;
  _sockID = -1;
}

/*
* execute ->
* Opens a server on a different thread that the simulator connects to, and recieves messages from it
* at a given speed.
* */
int DataReaderServer::execute(vector<string>& arguments, unsigned int index) {
  if ((arguments.size() - 1) < _argumentsAmount)
    throw "Amount of arguments is lower than " + to_string(_argumentsAmount);
  _port = (int) Evaluator::evaluate(arguments, &(++index), _symbolTable);
  if (_port < MIN_PORT_SIZE || _port > MAX_PORT_SIZE)
    throw "First argument must be in range of 1-65536";
  this->openSocket();
  thread t1(startServer, this->_sockID, 10, _pathToVar, _modifier);
  t1.detach();
  return index + 2;
}

/*
* opens a socket and waits for first signs from the simulator.
* */
void DataReaderServer::openSocket() {
  int server_fd;
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "Could not open server socket, CLI is terminated" << endl;
    exit(1);
  }
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(_port);
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    throw "Could not bind";
    exit(-1);
  }
  if (listen(server_fd, 5) < 0) {
    cout << "Could not listen, please be more quiet, CLI is terminated" << endl;
    exit(1);
  }
  _sockID = accept(server_fd, (struct sockaddr*) &address, &addrlen);
  if (_sockID < 0) {
    cout << "Could not accept a client, CLI is terminated." << endl;
    exit(1);
  }
  // cout << "Server has now accepted this client: " << address.sin_addr.s_addr << ", " << address.sin_port << endl;
}

/*
* starts the server on a different thread that runs on the
* speed given in the arguments.
* */
void DataReaderServer::startServer(int new_socket,
                                   unsigned int speed,

                                   map<string, string>* pathToVar,
                                   Modifier* modifier) {
  vector<string> names = getNames();
  char buffer[1024];
  // mutex mtx;
  while (true) {
    auto start = chrono::steady_clock::now();
    unsigned int i = 0;
    string receivedData = "";
    while (i < speed) {
      bzero(buffer, 1024);
      // mtx.lock();
      read(new_socket, buffer, 1024);
      receivedData += string(buffer);
      if (receivedData.find("\n") != string::npos) {
        string packet = receivedData.substr(0, receivedData.find("\n") + 1);
        receivedData = receivedData.substr(receivedData.find("\n") + 1);
        vector<string> valuesInString = StringHelper::split(packet, ",");
        vector<double> values;
        for (string value : valuesInString) {
          value.erase(remove_if(value.begin(), value.end(), ::isspace), value.end());
          values.push_back(stod(value));
        }
        updateVars(values, modifier, pathToVar, names);
        // mtx.unlock();
        i++;
      }
    }
    auto end = chrono::steady_clock::now();
    int time_passed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    int time_left = SECOND_IN_MILLI - time_passed;
    if (time_left > 0)
      this_thread::sleep_for(chrono::milliseconds(time_left));
  }
}

/*
* calls the modifier (who is responsible to managing the variables)
* and updates them accourding to the simulator.
* */
void DataReaderServer::updateVars(vector<double> values,
                                  Modifier* modifier,
                                  map<string, string>* pathToVar,
                                  vector<string>& names) {
  for (unsigned int i = 0; i < names.size(); i++) {
    string var = pathToVar->operator[](names[i]);
    modifier->setVariableValue(var, values[i]);
  }
}

vector<string> DataReaderServer::getNames() {
  vector<string> names = {"/instrumentation/airspeed-indicator/indicated-speed-kt",
                          "/sim/time/warp",
                          "/controls/switches/magnetos",
                          "//instrumentation/heading-indicator/offset-deg",
                          "/instrumentation/altimeter/indicated-altitude-ft",
                          "/instrumentation/altimeter/pressure-alt-ft",
                          "/instrumentation/attitude-indicator/indicated-pitch-deg",
                          "/instrumentation/attitude-indicator/indicated-roll-deg",
                          "/instrumentation/attitude-indicator/internal-pitch-deg",
                          "/instrumentation/attitude-indicator/internal-roll-deg",
                          "/instrumentation/encoder/indicated-altitude-ft",
                          "/instrumentation/encoder/pressure-alt-ft",
                          "/instrumentation/gps/indicated-altitude-ft",
                          "/instrumentation/gps/indicated-ground-speed-kt",
                          "/instrumentation/gps/indicated-vertical-speed",
                          "/instrumentation/heading-indicator/indicated-heading-deg",
                          "/instrumentation/magnetic-compass/indicated-heading-deg",
                          "/instrumentation/slip-skid-ball/indicated-slip-skid",
                          "/instrumentation/turn-indicator/indicated-turn-rate",
                          "/instrumentation/vertical-speed-indicator/indicated-speed-fpm",
                          "/controls/flight/aileron",
                          "/controls/flight/elevator",
                          "/controls/flight/rudder",
                          "/controls/flight/flaps",
                          "/controls/engines/engine/throttle",
                          "/controls/engines/current-engine/throttle",
                          "/controls/switches/master-avionics",
                          "/controls/switches/starter",
                          "/engines/active-engine/auto-start",
                          "/controls/flight/speedbrake",
                          "/sim/model/c172p/brake-parking",
                          "/controls/engines/engine/primer",
                          "/controls/engines/current-engine/mixture",
                          "/controls/switches/master-bat",
                          "/controls/switches/master-alt",
                          "/engines/engine/rpm"};
  return names;
}
