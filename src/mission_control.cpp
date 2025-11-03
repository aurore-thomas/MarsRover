#include <iostream>

#include "mission_control.hpp"
#include "console.hpp"

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
    // MissionControl missionControl;

    // int serverSocket = missionControl.LaunchServer();

    // while (true)
    // {
    //     string command = AskCommand();

    //     if (command == "exit")
    //     {
    //         break;
    //     }
    //     else if (IsValidCommand(command))
    //     {
    //         Packet packet;
    //         // packet.listInstructions = command;
    //         // missionControl.Send(packet);
    //     }
    //     else
    //     {
    //         cout << "Invalid command. Please use only F, B, L, R." << endl;
    //         continue;
    //     }


    //     // ECOUTER REPONSE ROVER
        
    // // char buffer[1024] = {0};
    // // recv(clientSocket, buffer, sizeof(buffer), 0);
    // // cout << "Message from client: " << buffer << endl;

    //     // DISPLAY MAP
    //     //     console.displayMap(planet.getWidth(), planet.getHeight(), planet.getMap(), NORTH);

    // }

    return 0;
}