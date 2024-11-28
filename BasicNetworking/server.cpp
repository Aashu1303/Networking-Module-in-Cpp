// server.cpp
#include "header/server.h"
#include <iostream>
#include <winsock2.h> // For Windows socket programming
#include <ws2tcpip.h>
#include <cstdlib>
#include <ctime>
#include <string>

#pragma comment(lib, "ws2_32.lib")

// Function to generate OTP
static std::string generateOTP()
{
    srand(static_cast<unsigned int>(time(0)));
    int otp = rand() % 10000; // Generate a 4-digit OTP
    char otpStr[5];
    sprintf_s(otpStr, "%04d", otp); // Ensure OTP has leading zeros
    return std::string(otpStr);
}

// Function to start the server
void startServer()
{
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);

    // Initialize winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed!" << std::endl;
        return;
    }

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed!" << std::endl;
        WSACleanup();
        return;
    }

    // Setup server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(443); // Port 443

    // Bind the socket to the address
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Server listening on port 443..." << std::endl;

    // Accept a client connection
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Accept failed!" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    std::cout << "Client connected!" << std::endl;

    // Generate OTP after client connection
    std::string otp = generateOTP();

    // Display OTP on server console
    std::cout << "Generated OTP: " << otp << std::endl;

    // Receive OTP from client
    char buffer[5] = { 0 };
    int bytesReceived = recv(clientSocket, buffer, 4, 0);
    if (bytesReceived <= 0)
    {
        std::cerr << "Failed to receive OTP from client!" << std::endl;
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    buffer[bytesReceived] = '\0';

    // Check OTP and send acknowledgment
    if (otp == buffer)
    {
        std::cout << "OTP verified successfully!" << std::endl;
        // Send acknowledgment to client
        std::string response = "OTP verified successfully!";
        send(clientSocket, response.c_str(), static_cast<int>(response.size()), 0);
    }
    else
    {
        std::cerr << "OTP verification failed!" << std::endl;
        // Send acknowledgment to client
        std::string response = "OTP verification failed!";
        send(clientSocket, response.c_str(), static_cast<int>(response.size()), 0);
    }

    // Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}
