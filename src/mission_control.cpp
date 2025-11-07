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
    if (!Bind(getPort())) {
        std::cerr << "Bind failed on port " << getPort() << std::endl;
        return false;
    }

    if (listen(getServerSocket().getSock(), 1) != 0) {
        std::cerr << "Listen failed" << std::endl;
        return false;
    }

    int clientSock = Accept();
    if (clientSock < 0) {
        std::cerr << "Accept failed" << std::endl;
        return false;
    }

    return true;
}

bool MissionControl::Bind(unsigned short port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    return bind(getServerSocket().getSock(), (sockaddr*)&addr, sizeof(addr)) == 0;
}

int MissionControl::Accept() {
    int client = accept(getServerSocket().getSock(), nullptr, nullptr);
    if (client < 0) {
        return -1;
    }
    getServerSocket().setSock(client);
    return client;
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

int main(int argc, char* argv[])
{
    Console console;
    UnixSocket serverSocket = UnixSocket();

    if (argc != 2) {
        std::cerr << "Usage: mission_control [port]" << std::endl;
        return 1;
    }
    unsigned short port = std::stoi(argv[1]);

    MissionControl missionControl = MissionControl(port, serverSocket);

    Packet clientPacket;
    int oldRoverX = -1;
    int oldRoverY = -1;
    if (missionControl.getServerSocket().Receive(clientPacket)) {
        RoverPacket rp = clientPacket.getRoverPacket();
        Planet unknownPlanet = Planet(rp.planetWidth, rp.planetHeight);
        unknownPlanet.setMap(unknownPlanet.createMapMissionControl(unknownPlanet.getWidth(), unknownPlanet.getHeight()));

        for (const auto& tile : rp.tilesDiscovered) {
            ObjectType type = StringToObjectType(tile.type);
            unknownPlanet.updateMapWithDiscoveredTiles(tile.x, tile.y, type);
            oldRoverX = tile.x;
            oldRoverY = tile.y;
        }
        cout << "Initial rover position: (" << oldRoverX << ", " << oldRoverY << ") and orientation: " << rp.orientation << endl;



        console.displayMap(unknownPlanet.getWidth(), unknownPlanet.getHeight(), unknownPlanet.getMap(), static_cast<Orientation>(rp.orientation));
        
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

            // Build a MissionControlPacket and wrap it into Packet
            MissionControlPacket mcp;
            mcp.finished = false;
            mcp.listInstructions = command;
            Packet commandPacket;
            commandPacket.setMissionControlPacket(mcp);

            if (!missionControl.getServerSocket().Send(commandPacket)) {
                std::cerr << "Send command failed" << std::endl;
                break;
            }

            Packet responsePacket;
            if (!missionControl.getServerSocket().Receive(responsePacket)) {
                std::cerr << "Receive response failed" << std::endl;
                break;
            }

            // response is expected to be a RoverPacket
            RoverPacket rpResp = responsePacket.getRoverPacket();
            for (const auto& tile : rpResp.tilesDiscovered) {
                cout << "(" << tile.x << ", " << tile.y << "): " << tile.type << endl;
                unknownPlanet.updateMapWithDiscoveredTiles(tile.x, tile.y, StringToObjectType(tile.type));
            }

            // Update rover position: clear old and set new
            if (oldRoverX >= 0 && oldRoverY >= 0) {
                unknownPlanet.updateMapWithDiscoveredTiles(oldRoverX, oldRoverY, EMPTY);
            }
            unknownPlanet.updateMapWithDiscoveredTiles(rpResp.roverX, rpResp.roverY, ROVER);

            cout << "Rover position: (" << rpResp.roverX << ", " << rpResp.roverY << ")" << endl;
            cout << "Rover old position: (" << oldRoverX << ", " << oldRoverY << ")" << endl;
            oldRoverX = rpResp.roverX;
            oldRoverY = rpResp.roverY;

            // console.displayMap expects an Orientation enum in previous code; pass 0 or adapt
            console.displayMap(unknownPlanet.getWidth(), unknownPlanet.getHeight(), unknownPlanet.getMap(), static_cast<Orientation>(rpResp.orientation));
        }

        if (!unknownPlanet.hasUnknownTiles()) {
            cout << "All tiles have been discovered. Mission complete!" << endl;
            // send mission complete
            MissionControlPacket mcp;
            mcp.finished = true;
            mcp.listInstructions = "";
            Packet finishPacket;
            finishPacket.setMissionControlPacket(mcp);
            missionControl.getServerSocket().Send(finishPacket);
        }

    } else {
        std::cerr << "Receive failed or no data" << std::endl;
        return 1;
    }

    missionControl.getServerSocket().~UnixSocket();
    return 0;
}