#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main() {
    // Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Error creating server socket." << endl;
        return 1;
    }

    // Bind the socket to a port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cerr << "Error binding socket." << endl;
        close(serverSocket);
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == -1) {
        cerr << "Error listening for connections." << endl;
        close(serverSocket);
        return 1;
    }

    cout << "Server listening on port 12345..." << endl;

    // Accept incoming connections
    int clientSocket;
    sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket == -1) {
        cerr << "Error accepting client connection." << endl;
        close(serverSocket);
        return 1;
    }

    cout << "Client connected." << endl;

    // Communication loop
    char buffer[256];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == -1) {
            cerr << "Error reading from client." << endl;
            break;
        }
        if (bytesRead == 0) {
            cout << "Client disconnected." << endl;
            break;
        }
        cout << "Received: " << buffer << endl;

        // Echo the message back to the client
        send(clientSocket, buffer, bytesRead, 0);
    }

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
