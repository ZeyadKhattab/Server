#pragma once

#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

using std::cout;
using std::cerr;
using std::string;

extern const int PORT;

int createSocket();

std::pair<int, fd_set> startServer();

void startClient();

void acceptConnectionsAndRespondToClients(int listeningSocket, fd_set &currentSockets);

void sendToSocket(int socket, string message);

