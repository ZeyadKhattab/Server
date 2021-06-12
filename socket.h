#pragma once

#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

using std::cout;
using std::cerr;
using std::string;

const int PORT = 4451;

int createSocket();

std::pair<int, fd_set> startServer();

void startClient();

void acceptConnections(int listeningSocket);
void respondToClients(int threadId);
