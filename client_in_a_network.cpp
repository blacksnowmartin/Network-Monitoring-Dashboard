#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Error creating client socket." << endl;
        return 1;
    }

    // Connect to the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0) {
        cerr << "Invalid server address." << endl;
        close(clientSocket);
        return 1;
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cerr << "Error connecting to server." << endl;
        close(clientSocket);
        return 1;
    }

    cout << "Connected to server." << endl;

    // Communication loop
    char buffer[256];
    while (true) {
        cout << "Enter a message (or 'exit' to quit): ";
        cin.getline(buffer, sizeof(buffer));
        
        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        send(clientSocket, buffer, strlen(buffer), 0);
        
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead == -1) {
            cerr << "Error reading from server." << endl;
            break;
        }
        if (bytesRead == 0) {
            cout << "Server disconnected." << endl;
            break;
        }
        cout << "Server response: " << buffer << endl;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
