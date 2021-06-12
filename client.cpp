#include "socket.h"
#include <vector>


void startClient(const int NUM_CLIENTS) {

    std::vector<int> connectedSockets;
    for (int i = 0; i < NUM_CLIENTS; i++) {

        int socket = createSocket();
        sockaddr_in server;
        inet_aton("127.0.0.1", &server.sin_addr);
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        if (connect(socket, (sockaddr *) &server, sizeof(server)) == -1) {
            cerr << "Connection error\n";
            return;
        }
        connectedSockets.push_back(socket);

    }

    for (int socket:connectedSockets) {
        sendToSocket(socket, "hello");
    }
}


