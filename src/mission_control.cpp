#include <iostream>
#include <cstring>

#include "mission_control.hpp"
#include "console.hpp"

#include "common/win_socket.hpp"
#include "common/unix_socket.hpp"
#include "common/packet.hpp"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#endif

using namespace std;

string AskCommand()
{
    string command;
    cout << "Enter command (F: Forward, B: Backward, L: Left, R: Right, exit to quit): ";
    cin >> command;
    return command;
}

bool IsValidCommand(const string &command)
{
    for (char c : command)
    {
        if (c != 'F' && c != 'B' && c != 'L' && c != 'R')
        {
            return false;
        }
    }
    return true;
}

int main()
{
    Console console;
    const unsigned short port = 8080;

#ifdef _WIN32
    WinSocket server;
#else
    UnixSocket server;
#endif

    if (!server.Init()) {
        std::cerr << "Socket init failed" << std::endl;
        return 1;
    }

    if (!server.Create()) {
        std::cerr << "Socket create failed" << std::endl;
        return 1;
    }

    if (!server.Bind(port)) {
        std::cerr << "Bind failed on port " << port << std::endl;
        return 1;
    }

    if (!server.Listen(1)) {
        std::cerr << "Listen failed" << std::endl;
        return 1;
    }

    std::cout << "Server listening on port " << port << ", waiting for client..." << std::endl;

    int clientSock = server.Accept();
    if (clientSock < 0) {
        std::cerr << "Accept failed" << std::endl;
        return 1;
    }

    // Prepare a Packet containing the message and send using the socket wrapper
    Packet packet;
    packet.setListInstructions(std::string("hello word"));

    if (!server.Send(packet)) {
        std::cerr << "Send packet failed" << std::endl;
    } else {
        std::cout << "Sent packet to client" << std::endl;
    }

    // Close the connection
#ifdef _WIN32
    server.Close();
    server.Cleanup();
#else
    server.Close();
#endif

    return 0;
}