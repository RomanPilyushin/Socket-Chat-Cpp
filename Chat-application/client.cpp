#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Use the same port as the server
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Use the server's IP

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    char message[1024];
    while (true) {
        std::cout << "You: ";
        std::cin.getline(message, sizeof(message));

        send(clientSocket, message, static_cast<int>(strlen(message)), 0);

        memset(message, 0, sizeof(message));
        int bytesReceived = recv(clientSocket, message, sizeof(message), 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            std::cerr << "Server disconnected." << std::endl;
            break;
        }
        std::cout << "Server: " << message << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
