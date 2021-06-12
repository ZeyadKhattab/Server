#include <cstring>
#include <unordered_set>
#include <vector>
#include "socket.h"
#include <fstream>

const int PORT = 4441;

void bindAndListen(int listeningSocket) {

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    inet_aton("0.0.0.0", &hint.sin_addr);


    if (bind(listeningSocket, (sockaddr *) &hint, sizeof(hint)) == -1) {
        cerr << "Error in binding\n";
        exit(EXIT_FAILURE);
    }


    if (listen(listeningSocket, SOMAXCONN) == -1) {
        cerr << "Error in listening\n";
        exit(EXIT_FAILURE);
    }

}

std::pair<int, fd_set> startServer() {

    int listeningSocket = createSocket();
    bindAndListen(listeningSocket);

    cout << "Server running on port " << PORT << "\n";

    fd_set currentSockets;
    FD_ZERO(&currentSockets);
    FD_SET(listeningSocket, &currentSockets);


    return {listeningSocket, currentSockets};

}

int createSocket() {
    int listeningSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (listeningSocket == -1) {
        cerr << "Can't create a socket! Quitting\n";
        exit(EXIT_FAILURE);
    }

    return listeningSocket;
}


int acceptConnection(int listeningSocket) {
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);


    int clientSocket = accept(listeningSocket, (sockaddr *) &client, &clientSize);

    return clientSocket;
}

string receiveFromClient(int clientSocket) {
    const static int N = 4096;
    char buf[N];
    memset(buf, 0, N);
    int bytesReceived = recv(clientSocket, buf, N, 0);
    if (bytesReceived <= 0) {
        return "";
    }

    return string(buf, 0, bytesReceived);


}

void sendToSocket(int clientSocket, string message) {
    send(clientSocket, message.c_str(), message.size() + 1, 0);
}

string htmlToString() {

    string content, html = "";
    std::ifstream htmlFile("../index.html");
    if (htmlFile.is_open()) {
        string line;
        while (getline(htmlFile, line)) {
            content += line + "\n";
        }
    } else {
        cerr << "Error in reading file\n";
        exit(EXIT_FAILURE);
    }

    html += ("HTTP/1.1 " + string("200") + " OK\r\n");
    html += "Content-Type: text/html\r\n";
    html += "\r\n";
    html += content;
    return html;
}

void cleanup(std::unordered_set<int> socketSet, int listeningSocket) {

    for (int socket:socketSet)
        close(socket);
    close(listeningSocket);

}


void acceptConnectionsAndRespondToClients(int listeningSocket, fd_set &currentSockets) {
    const string html = htmlToString();
    std::unordered_set<int> socketSet;
    socketSet.insert(listeningSocket);
    std::vector<int> socketsToAdd, socketsToRemove;
    timeval timeout;
    timeout.tv_sec = 20;
    timeout.tv_usec = 0;
    while (1) {
        fd_set copy = currentSockets;
        int selectValue = select(FD_SETSIZE, &copy, nullptr, nullptr, &timeout);
        if (selectValue < 0) {
            cerr << " Select error\n";
            exit(EXIT_FAILURE);
        }
        if (selectValue == 0) {
            cout << "Server has been inactive and is now terminating\n";
            break;
        }
        for (int i:socketSet)
            if (FD_ISSET(i, &copy)) {
                if (i == listeningSocket) {
                    int clientSocket = acceptConnection(listeningSocket);
                    if (clientSocket == -1) {
                        cerr << "Could not connect";
                        break;
                    }
                    FD_SET(clientSocket, &currentSockets);
                    socketsToAdd.push_back(clientSocket);
                    cout << "Client " << clientSocket << " connected\n";
                } else {
                    string messageFromClient = receiveFromClient(i);
                    if (messageFromClient == "") {
                        FD_CLR(i, &currentSockets);
                        socketsToRemove.push_back(i);
                    } else {
                        cout << "Message from: " << i << " " << messageFromClient << "\n";
                        sendToSocket(i, html);

                    }
                }
            }

        for (int socket:socketsToAdd)
            socketSet.insert(socket);
        socketsToAdd.clear();

        for (int socket:socketsToRemove)
            socketSet.erase(socket);
        socketsToRemove.clear();

    }
    cleanup(socketSet, listeningSocket);

}

