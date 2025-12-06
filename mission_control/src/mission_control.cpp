#include <iostream>
#include <cstring>

#include "mission_control.hpp"

using namespace std;

MissionControl::MissionControl(unsigned short port)
    : port(port), server()
{
    //LaunchServer(); on le lance dans le main en bas 
}

bool MissionControl::LaunchServer() 
{
    if (!Bind(port)) {
        std::cerr << "Bind failed on port " << port << std::endl;
        return false;
    }

    if (listen(server.getSock(), 1) != 0) {
        std::cerr << "Listen failed" << std::endl;
        return false;
    }

    std::cout << "Mission Control is listening on port " << port << std::endl;

    int clientSock = Accept();
    if (clientSock < 0) {
        std::cerr << "Accept failed" << std::endl;
        return false;
    }

    std::cout << "Rover connected" << std::endl;

    return true;
}

bool MissionControl::Bind(unsigned short port) {
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);
    int sock = server.getSock();
    if (sock < 0) {
        std::perror("socket");
        return false;
    }

    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::perror("setsockopt");
        // continue even if setsockopt fails
    }

    if (bind(sock, (sockaddr*)&addr, sizeof(addr)) != 0) {
        std::perror("bind");
        return false;
    }
    return true;
}

int MissionControl::Accept() {
    int client = accept(server.getSock(), nullptr, nullptr);
    if (client < 0) {
        return -1;
    }
    server.setSock(client);
    return client;
}

string MissionControl::AskCommand()
{
    string command;
    cout << "Enter command (F: Forward, B: Backward, L: Left, R: Right, exit to quit): ";
    cin >> command;

    return command;
}

bool MissionControl::IsValidCommand(const string &command)
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

ObjectType MissionControl::StringToObjectType(const string &typeStr)
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

bool MissionControl::HasUnknownTiles(Tile **map, int width, int height) const 
{
  for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
          if (map[x][y].type == UNKNOWN) return true;
      }
  }
  return false;
}

void MissionControl::UpdateMapWithDiscoveredTiles(Tile **map, const int x, const int y, const ObjectType type) 
{
  map[x][y].type = type;
}

void MissionControl::DisplayMap(int width, int height, Tile **map, Orientation orientation) const
{
  cout << "Mars map:" << endl << endl;

  string separationRow = "";
  for (int x = 0; x < width; x++)
  {
    separationRow += "####";
  }
  separationRow += "#";

  for (int y = height - 1; y >= 0; y--) {
    cout << separationRow << endl;

    for (int x = 0; x < width; x++) {
      string cellContent = "";

      if (map[x][y].type == ROVER)
      {
        if (orientation == NORTH) { cellContent = " ^ "; }
        else if (orientation == EAST)  { cellContent = " > "; }
        else if (orientation == SOUTH) { cellContent = " v "; }
        else if (orientation == WEST)  { cellContent = " < "; }
      }
      else if (map[x][y].type == UNKNOWN) {
        cellContent = " ? ";
      }
      else if (map[x][y].type == EMPTY)
      {
        cellContent = " . ";
      }
      else if (map[x][y].type == OBSTACLE)
      {
        cellContent = " X ";
      }

        cout << "#" << cellContent;
    }
    cout << "#" << endl;
  }
  cout << endl; 
}

void MissionControl::Main() 
{
    // On lance le serveur ici, juste avant de commencer la boucle principale
    if (!LaunchServer()) {
        std::cerr << "Failed to launch server. Exiting." << std::endl;
        return;
    }

    PacketSerializer clientPacket;
    int oldRoverX = -1;
    int oldRoverY = -1;
    if (server.Receive(clientPacket)) {
        RoverPacket roverPacket = clientPacket.getRoverPacket();
        Planet unknownPlanet = Planet(roverPacket.planetWidth, roverPacket.planetHeight, true);

        for (const auto& tile : roverPacket.tilesDiscovered) {
            ObjectType type = StringToObjectType(tile.type);
            UpdateMapWithDiscoveredTiles(unknownPlanet.getMap(), tile.x, tile.y, type);
            oldRoverX = tile.x;
            oldRoverY = tile.y;
        }

        DisplayMap(unknownPlanet.getWidth(), unknownPlanet.getHeight(), unknownPlanet.getMap(), static_cast<Orientation>(roverPacket.orientation));
        
        while (HasUnknownTiles(unknownPlanet.getMap(), unknownPlanet.getWidth(), unknownPlanet.getHeight())) 
        {
            string command = AskCommand();

            if (command == "exit") {
                break;
            }
            if (!IsValidCommand(command)) {
                cout << "Invalid command. Please enter a valid command." << endl;
                continue;
            }

            // Build a MissionControlPacket and wrap it into Packet
            MissionControlPacket missionsControlPacket;
            missionsControlPacket.finished = false;
            missionsControlPacket.listInstructions = command;
            PacketSerializer commandPacket;
            commandPacket.setMissionControlPacket(missionsControlPacket);

            
            if (!server.Send(commandPacket)) {
                std::cerr << "Send command failed" << std::endl;
                break;
            }
            
            PacketSerializer responsePacket;
            if (!server.Receive(responsePacket)) {
                std::cerr << "Receive response failed" << std::endl;
                break;
            }
            
            RoverPacket roverPacketResp = responsePacket.getRoverPacket();
            for (const auto& tile : roverPacketResp.tilesDiscovered) {
                UpdateMapWithDiscoveredTiles(unknownPlanet.getMap(), tile.x, tile.y, StringToObjectType(tile.type));
            }
            
            if (oldRoverX >= 0 && oldRoverY >= 0) {
                UpdateMapWithDiscoveredTiles(unknownPlanet.getMap(), oldRoverX, oldRoverY, EMPTY);
            }
            UpdateMapWithDiscoveredTiles(unknownPlanet.getMap(), roverPacketResp.roverX, roverPacketResp.roverY, ROVER);

            oldRoverX = roverPacketResp.roverX;
            oldRoverY = roverPacketResp.roverY;

            DisplayMap(unknownPlanet.getWidth(), unknownPlanet.getHeight(), unknownPlanet.getMap(), static_cast<Orientation>(roverPacketResp.orientation));
        }

        if (!HasUnknownTiles(unknownPlanet.getMap(), unknownPlanet.getWidth(), unknownPlanet.getHeight())) {
            cout << "All tiles have been discovered. Mission complete!" << endl;
            // send mission complete
            MissionControlPacket missionsControlPacket;
            missionsControlPacket.finished = true;
            missionsControlPacket.listInstructions = "";
            PacketSerializer finishPacket;
            finishPacket.setMissionControlPacket(missionsControlPacket);
            server.Send(finishPacket);
        }
    } else {
        std::cerr << "Receive failed or no data" << std::endl;
        return;
    }
}

