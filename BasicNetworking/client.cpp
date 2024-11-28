// client.cpp
#include "header/client.h"
#include <iostream>
#include <winsock2.h> // For Windows socket programming
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

// Function to start the client
void startClient()
{
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    std::string ipAddress;
    int port;

    std::cout << "Enter IP address: ";
    std::cin >> ipAddress;
    std::cout << "Enter port: ";
    std::cin >> port;

    // Initialize winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed!" << std::endl;
        return;
    }

    // Create a socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return;
    }

    // Setup server address
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr); // Convert IP address
    serverAddr.sin_port = htons(port);                           // Convert port to network byte order

    // Connect to the server
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Connection failed!" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    std::cout << "Connected to the server!" << std::endl;

    // Prompt user to enter OTP
    std::string enteredOtp;
    std::cout << "Enter OTP: ";
    std::cin >> enteredOtp;

    // Send OTP to server
    send(clientSocket, enteredOtp.c_str(), enteredOtp.size(), 0);

    // Optionally, receive verification result from server
    char buffer[1024] = {0};
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0)
    {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        std::cout << "Server response: " << buffer << std::endl;
    }
    else
    {
        std::cerr << "Failed to receive response from server!" << std::endl;
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
}
