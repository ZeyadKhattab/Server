#include "socket.h"

void startClient() {
    for (int i = 0; i < 506; i++) {

        int socket = createSocket();
        sockaddr_in server;
        inet_aton("127.0.0.1", &server.sin_addr);
        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        if (connect(socket, (sockaddr *) &server, sizeof(server)) == -1) {
            cerr << "Connection error\n";
            return;
        }

    }
}

