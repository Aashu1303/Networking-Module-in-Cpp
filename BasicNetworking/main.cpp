// BasicNetworking.cpp
#include "header/server.h"
#include "header/client.h"
#include <iostream>

int main()
{
    int choice;

    std::cout << "Select mode: 1 for Server, 2 for Client: ";
    std::cin >> choice;

    if (choice == 1)
    {
        startServer();
    }
    else if (choice == 2)
    {
        startClient();
    }
    else
    {
        std::cerr << "Invalid choice!" << std::endl;
    }
    return 0;
}
