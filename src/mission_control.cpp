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

    //Listen first client packet
    Packet clientPacket;
    if (!missionControl.getServerSocket().Receive(clientPacket)) {
        std::cerr << "Receive failed or no data" << std::endl;
        return 1;
    }
    else {
        Planet unknownPlanet = Planet(clientPacket.getPacketPlanetWidth(), clientPacket.getPacketPlanetHeight());
        unknownPlanet.setMap(unknownPlanet.createMapMissionControl(unknownPlanet.getWidth(), 
            unknownPlanet.getHeight(), clientPacket.getPacketPositionX(), clientPacket.getPacketPositionY()));
        console.displayMap(unknownPlanet.getWidth(), unknownPlanet.getHeight(), unknownPlanet.getMap(), clientPacket.getPacketOrientation());
    }

    

    // Prepare a Packet containing the message and send using the socket wrapper
    // Packet packet;
    // packet.setListInstructions(std::string("hello word"));

    // if (!missionControl.getServerSocket().Send(packet)) {
    //     std::cerr << "Send packet failed" << std::endl;
    // } else {
    //     std::cout << "Sent packet to client" << std::endl;
    // }

    missionControl.getServerSocket().Close();

    return 0;
}