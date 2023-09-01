#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Choose a port
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    listen(serverSocket, 5);

    std::cout << "Server listening on port 12345..." << std::endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);

        char message[1024];
        while (true) {
            memset(message, 0, sizeof(message));
            int bytesReceived = recv(clientSocket, message, sizeof(message), 0);
            if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
                std::cerr << "Client disconnected." << std::endl;
                closesocket(clientSocket);
                break;
            }
            std::cout << "Client: " << message << std::endl;

            // Echo the message back to the client
            send(clientSocket, message, bytesReceived, 0);
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
