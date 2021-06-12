#include <tuple>
#include "socket.h"

int main() {
    
    int listeningSocket;
    fd_set currentSockets;
    std::tie(listeningSocket, currentSockets) = startServer();
    startClient();
    acceptConnectionsAndRespondToClients(listeningSocket, currentSockets);
}