#include <iostream>

#include "../include/mission_control.hpp"
#include "../include/console.hpp"

using namespace std;

MissionControl::MissionControl() : Communication()
{
    LaunchServer();
}

int MissionControl::LaunchServer() const
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    listen(serverSocket, 1);

    int clientSocket = accept(serverSocket, nullptr, nullptr);

    // char buffer[1024] = {0};
    // recv(clientSocket, buffer, sizeof(buffer), 0);
    // cout << "Message from client: " << buffer << endl;
};

void MissionControl::CloseServer(SOCKET serverSocket) const
{
    close(serverSocket);
}

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
    MissionControl missionControl;

    int serverSocket = missionControl.LaunchServer();

    while (true)
    {
        string command = AskCommand();

        if (command == "exit")
        {
            break;
        }
        else if (IsValidCommand(command))
        {
            Packet packet;
            packet.listInstructions = command;
            missionControl.Send(packet);
        }
        else
        {
            cout << "Invalid command. Please use only F, B, L, R." << endl;
            continue;
        }


        // ECOUTER REPONSE ROVER
        
    // char buffer[1024] = {0};
    // recv(clientSocket, buffer, sizeof(buffer), 0);
    // cout << "Message from client: " << buffer << endl;

        // DISPLAY MAP
        //     console.displayMap(planet.getWidth(), planet.getHeight(), planet.getMap(), NORTH);

    }

    return 0;
}