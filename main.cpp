#include <tuple>
#include <string>
#include "socket.h"

int PORT;

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cout << "Must provide Port as a command line argument\n";
        exit(0);
    }
    PORT = std::stoi(argv[1]);
    int listeningSocket;
    fd_set currentSockets;
    std::tie(listeningSocket, currentSockets) = startServer();
    startClient(argc >= 3 ? std::stoi(argv[2]) : 0);
    acceptConnectionsAndRespondToClients(listeningSocket, currentSockets);
}
