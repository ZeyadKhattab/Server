#pragma once

#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

using std::cout;
using std::cerr;
using std::string;

extern int PORT;

int createSocket();

std::pair<int, fd_set> startServer();

void startClient(const int numClients);

void acceptConnectionsAndRespondToClients(int listeningSocket, fd_set &currentSockets);

void sendToSocket(int socket, string message);

