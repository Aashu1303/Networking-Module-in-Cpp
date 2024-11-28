#include <iostream>
#include <winsock2.h> // For Windows socket programming
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

// Function to start the server
void startServer()
{
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);


	WSAStartup(MAKEWORD(2, 2), &wsaData); // Initialize winsock


    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed!" << std::endl;
        return;
    }

    // Setup server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(443); // Port 8080

    // Bind the socket to the address
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed!" << std::endl;
        return;
    }

    // Listen for incoming connections
    listen(serverSocket, 1);
    std::cout << "Server listening on port 443..." << std::endl;

    // Accept a client connection
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Accept failed!" << std::endl;
        return;
    }

    std::cout << "Client connected!" << std::endl;

    // Cleanup
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

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

    WSAStartup(MAKEWORD(2, 2), &wsaData); // Initialize winsock

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

    // Cleanup
    closesocket(clientSocket);
    WSACleanup();
}
