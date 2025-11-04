#include <iostream>
#include <cstring>

#include "mission_control.hpp"
#include "console.hpp"

using namespace std;

MissionControl::MissionControl(unsigned short port, UnixSocket& serverSocket) 
{
    setPort(port);
    setServerSocket(serverSocket);
    LaunchServer();
}

void MissionControl::setPort(unsigned short port) 
{
    this->port = port;
}
unsigned short MissionControl::getPort() const 
{
    return this->port;
}
void MissionControl::setServerSocket(UnixSocket& serverSocket) 
{
    this->server = serverSocket;
}
UnixSocket& MissionControl::getServerSocket() 
{
    return this->server;
}

bool MissionControl::LaunchServer() 
{
    if (!getServerSocket().Init()) {
        std::cerr << "Socket init failed" << std::endl;
        return false;
    }

    if (!getServerSocket().Create()) {
        std::cerr << "Socket create failed" << std::endl;
        return false;
    }

    if (!getServerSocket().Bind(getPort())) {
        std::cerr << "Bind failed on port " << getPort() << std::endl;
        return false;
    }

    if (!getServerSocket().Listen(1)) {
        std::cerr << "Listen failed" << std::endl;
        return false;
    }

    return true;
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

ObjectType StringToObjectType(const string &typeStr)
{
    if (typeStr == "ROVER")
    {
        return ROVER;
    }
    else if (typeStr == "OBSTACLE")
    {
        return OBSTACLE;
    }
    else if (typeStr == "EMPTY")
    {
        return EMPTY;
    }
    else if (typeStr == "UNKNOWN")
    {
        return UNKNOWN;
    }
    return UNKNOWN;
} 

int main()
{
    Console console;
    UnixSocket serverSocket;

    MissionControl missionControl = MissionControl(8080, serverSocket);

    int clientSock = missionControl.getServerSocket().Accept();
    if (clientSock < 0) {
        std::cerr << "Accept failed" << std::endl;
        return 1;
    }

    Packet clientPacket;
    int oldRoverX = -1;
    int oldRoverY = -1;
    if (missionControl.getServerSocket().Receive(clientPacket)) {
        Planet unknownPlanet = Planet(clientPacket.getPacketPlanetWidth(), clientPacket.getPacketPlanetHeight());
        unknownPlanet.setMap(unknownPlanet.createMapMissionControl(unknownPlanet.getWidth(), unknownPlanet.getHeight()));

        for (const auto& tile : clientPacket.getTilesDiscovered()) {
            ObjectType type = StringToObjectType(tile.type);
            unknownPlanet.updateMapWithDiscoveredTiles(tile.x, tile.y, type);
            oldRoverX = tile.x;
            oldRoverY = tile.y;
        }

        console.displayMap(unknownPlanet.getWidth(), unknownPlanet.getHeight(), unknownPlanet.getMap(), clientPacket.getPacketOrientation());
        
        while (unknownPlanet.hasUnknownTiles()) 
        {
            string command = AskCommand();
            if (command == "exit") {
                // todo send exit command to rover?
                break;
            }

            if (!IsValidCommand(command)) {
                cout << "Invalid command. Please enter a valid command." << endl;
                continue;
            }

            Packet commandPacket;
            commandPacket.setFinished(false);
            commandPacket.setListInstructions(command);

            if (!missionControl.getServerSocket().Send(commandPacket)) {
                std::cerr << "Send command failed" << std::endl;
                break;
            }

            Packet responsePacket;
            if (!missionControl.getServerSocket().Receive(responsePacket)) {
                std::cerr << "Receive response failed" << std::endl;
                break;
            }

            for (const auto& tile : responsePacket.getTilesDiscovered()) {
                cout << "(" << tile.x << ", " << tile.y << "): " << tile.type << endl;
                unknownPlanet.updateMapWithDiscoveredTiles(tile.x, tile.y, StringToObjectType(tile.type));
            }

            //Update rover position
            unknownPlanet.updateMapWithDiscoveredTiles(responsePacket.getPacketRoverX(), responsePacket.getPacketRoverY(), ROVER);
            unknownPlanet.updateMapWithDiscoveredTiles(oldRoverX, oldRoverY, EMPTY);
            oldRoverX = responsePacket.getPacketRoverX();
            oldRoverY = responsePacket.getPacketRoverY();

            console.displayMap(unknownPlanet.getWidth(), unknownPlanet.getHeight(), unknownPlanet.getMap(), responsePacket.getPacketOrientation());
        }

        if (!unknownPlanet.hasUnknownTiles()) {
            cout << "All tiles have been discovered. Mission complete!" << endl;
            clientPacket.setFinished(true);
            missionControl.getServerSocket().Send(clientPacket);
        }

    } else {
        std::cerr << "Receive failed or no data" << std::endl;
        return 1;
    }

    missionControl.getServerSocket().Close();
    return 0;
}