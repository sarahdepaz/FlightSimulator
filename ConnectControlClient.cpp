#include "ConnectControlClient.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <thread>
#include <malloc.h>
#include <stdio.h>

#define MAX_PORT_SIZE 65536
#define MIN_PORT_SIZE 1
/**
*constructor.
*/
ConnectControlClient::ConnectControlClient(map<string, double> *symbolTable) {
  _symbolTable = symbolTable;
  _argumentsAmount = 4;
  _client_fd = -1;
}

/**
 * function that starts the connection between the simulator and program
 * as clients.
*/
void ConnectControlClient::startClient(const char *dst_addr) {
  close(_client_fd);
  struct sockaddr_in address;
  // tries to open a socket
  if ((_client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    cout << "Could not open client socket, CLI is terminated" << endl;
    exit(1);
  }
  // sets socket's attributes.
  address.sin_family = AF_INET;
  inet_aton(dst_addr, &address.sin_addr);
  address.sin_port = htons(_port);
  // tries to connect to simulator
  if (connect(_client_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
    cout << "Could not connect, CLI is terminated" << endl;
    exit(1);
  }
// cout << "Client socket is now connected to: " << address.sin_addr.s_addr << ", " << address.sin_port << endl;
}

/**
 * do Command reads the arguments and sets the connection
 * between the simulator.
 * */
int ConnectControlClient::execute(vector<string> &arguments, unsigned int index) {
  index += 2;
  if ((arguments.size() - 1) < _argumentsAmount)
    throw "Arguments amount is lower than " + to_string(_argumentsAmount);
  string ip_address = arguments[index];
  if (ip_address[0] == '\"')
    ip_address = arguments[index].substr(1, arguments[index].length() - 2);
  const char *ip_address_after_remove = ip_address.c_str();
  _port = (int) Evaluator::evaluate(arguments, &(++index), _symbolTable);
  if (_port < MIN_PORT_SIZE || _port > MAX_PORT_SIZE)
    throw "Second argument must be in range of 1-65536";
  startClient(ip_address_after_remove);
  return (index + 2);
}

void ConnectControlClient::sendMessage(const string message) {
  char *msg = NULL;
  while (msg == NULL)
    msg = (char *) malloc((message.length() + 3) * sizeof(char));
  strcpy(msg, message.c_str());
  strcat(msg, "\r\n");
  send(_client_fd, msg, strlen(msg), 0);
  free(msg);
}

ConnectControlClient::~ConnectControlClient() {
  close(_client_fd);
}
