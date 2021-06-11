#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <fstream>

using namespace std;

const int PORT = 4568;

int createSocket() {
    int listeningSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listeningSocket == -1) {
        cerr << "Can't create a socket! Quitting" << endl;
        exit(1);
    }
    return listeningSocket;
}

void bindAndListen(int listeningSocket) {
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(PORT);
    inet_aton("0.0.0.0", &hint.sin_addr);

    if (bind(listeningSocket, (sockaddr *) &hint, sizeof(hint)) == -1) {
        cerr << "Error in binding\n";
        exit(0);
    }

    if (listen(listeningSocket, SOMAXCONN) < 0) {
        cerr << "Error in listening\n";
        exit(0);
    }

}

int acceptConnection(int listeningSocket) {
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);


    int clientSocket = accept(listeningSocket, (sockaddr *) &client, &clientSize);
    return clientSocket;
}

string receiveFromClient(int clientSocket) {
    char buf[4096];
    memset(buf, 0, 4096);

    int bytesReceived = recv(clientSocket, buf, 4096, 0);
    if (bytesReceived == -1) {
        cerr << "Error in recv(). Quitting" << endl;
        exit(1);
    }

    if (bytesReceived == 0) {
        return "";
    }

    return string(buf, 0, bytesReceived);


}

void sendToClient(int clientSocket, string message) {
    send(clientSocket, message.c_str(), message.size() + 1, 0);
}

string htmlToString() {

    string content, html = "";
//    ifstream htmlFile("index.html");
//
//    while (getline(htmlFile, line)) {
//        cout << line;
//        content += line + "\n";
//    }

    html += ("HTTP/1.1 " + string("200") + " OK\r\n");
    html += "Cache-Control: no-cache, private\r\n";
    html += "Content-Type: text/html\r\n";
    html += "\r\n";
    content = "<!DOCTYPE html>\n"
              "<html lang=\"en\">\n"
              "<head>\n"
              "    <meta charset=\"UTF-8\">\n"
              "    <title>Sample</title>\n"
              "</head>\n"
              "<body>\n"
              "<h1>Hello World</h1>\n"
              "</body>\n"
              "</html>";
    html += content;
    return html;
}

int main() {


    const string html = htmlToString();
    int listeningSocket = createSocket();
    cout << "Server running on port" << PORT << "\n";


    bindAndListen(listeningSocket);


    fd_set currentSockets;
    FD_ZERO(&currentSockets);
    FD_SET(listeningSocket, &currentSockets);

    while (1) {
        fd_set copy = currentSockets;
        if (select(FD_SETSIZE, &copy, nullptr, nullptr, nullptr) < 0) {
            cerr << " Select error\n";
            exit(1);
        }
        for (int i = 0; i < FD_SETSIZE; i++)
            if (FD_ISSET(i, &copy)) {
                if (i == listeningSocket) {
                    int clientSocket = acceptConnection(listeningSocket);
                    FD_SET(clientSocket, &currentSockets);
                    cout << "Client " << clientSocket << " connected\n ";
//                    sendToClient(clientSocket, html);
                } else {
                    string messageFromClient = receiveFromClient(i);
                    if (messageFromClient == "")
                        FD_CLR(i, &currentSockets);
                    else {
                        cout << "Message from: " << i << " " << messageFromClient << "\n";
                        sendToClient(i, html);

                    }
                }
            }
    }


}