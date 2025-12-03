#include <iostream>
#include <random>

#include "rover.hpp"

using namespace std;

Rover::Rover(Planet &planet, const unsigned short port, string address)
    : client(), planet(planet)
{
    InitializeRoverPosition();
    if (!LaunchClient(port, address)) {
        std::cerr << "Connection to Mission Control failed" << std::endl;
    }
    else {
        std::cout << "Connected to Mission Control at " << address << ":" << port << std::endl;
    }
}

bool Rover::LaunchClient(const unsigned short port, string address) 
{
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, address.c_str(), &addr.sin_addr);

    return connect(client.getSock(), (sockaddr*)&addr, sizeof(addr)) == 0;
}

Orientation Rover::RotationHoraire(Orientation firstOrientation)
{
    switch (firstOrientation)
    {
    case NORTH:
        return EAST;
    case EAST:
        return SOUTH;
    case SOUTH:
        return WEST;
    case WEST:
        return NORTH;
    default:
        return NORTH;
    }
}

Orientation Rover::RotationAntiHoraire(Orientation firstOrientation)
{
    switch (firstOrientation)
    {
    case NORTH:
        return WEST;
    case EAST:
        return NORTH;
    case SOUTH:
        return EAST;
    case WEST:
        return SOUTH;
    default:
        return NORTH;
    }
}

int Rover::Modulo(int a, int b)
{
    return ((a % b) + b ) % b;
}

bool Rover::RoverMovement(RoverPacket &response, int multiplicator)
{
    int w = planet.getWidth();
    int h = planet.getHeight();

    if (orientation == NORTH)
    {
        if (planet.IsFreeTile(positionX, Modulo(positionY + 1 * multiplicator,h)))
        {
            positionY = Modulo(positionY + 1 * multiplicator,h);
            RoverPacket::TileDiscovered tile{positionX, positionY, "EMPTY"};
            response.tilesDiscovered.push_back(std::move(tile));
        }
        else
        {
            RoverPacket::TileDiscovered tile{positionX, Modulo(positionY + 1 * multiplicator,h), "OBSTACLE"};
            response.tilesDiscovered.push_back(std::move(tile));
            return false;
        }
    }
    else if (orientation == EAST)
    {
        if (planet.IsFreeTile(Modulo(positionX + 1 * multiplicator,w), positionY))
        {
            positionX = Modulo(positionX + 1 * multiplicator,w);
            RoverPacket::TileDiscovered tile{positionX, positionY, "EMPTY"};
            response.tilesDiscovered.push_back(std::move(tile));
        }
        else
        {
            RoverPacket::TileDiscovered tile{Modulo(positionX + 1 * multiplicator,w), positionY, "OBSTACLE"};
            response.tilesDiscovered.push_back(std::move(tile));
            return false;
        }
    }
    else if (orientation == SOUTH)
    {
        if (planet.IsFreeTile(positionX, Modulo(positionY - 1 * multiplicator,h)))
        {
            positionY = Modulo(positionY - 1 * multiplicator,h);
            RoverPacket::TileDiscovered tile{positionX, positionY, "EMPTY"};
            response.tilesDiscovered.push_back(std::move(tile));
        }
        else
        {
            RoverPacket::TileDiscovered tile{positionX, Modulo(positionY - 1 * multiplicator,h), "OBSTACLE"};
            response.tilesDiscovered.push_back(std::move(tile));
            return false;
        }
    }
    else if (orientation == WEST)
    {
        if (planet.IsFreeTile(Modulo(positionX - 1 * multiplicator,w), positionY))
        {
            positionX = Modulo(positionX - 1 * multiplicator,w);
            RoverPacket::TileDiscovered tile{positionX, positionY, "EMPTY"};
            response.tilesDiscovered.push_back(std::move(tile));
        }
        else
        {
            RoverPacket::TileDiscovered tile{Modulo(positionX - 1 * multiplicator,w), positionY, "OBSTACLE"};
            response.tilesDiscovered.push_back(std::move(tile));
            return false;
        }
    }
    return true;
}

Command Rover::ConvertCharToCommand(char commandChar)
{
    switch (commandChar)
    {
    case 'F':
        return MOVE_FORWARD;
    case 'B':
        return MOVE_BACKWARD;
    case 'L':
        return TURN_LEFT;
    case 'R':
        return TURN_RIGHT;
    default:
        throw std::invalid_argument("Invalid command character");
    }
}

RoverPacket Rover::ExecuteCommand(const string &command)
{
    RoverPacket response;
    bool obstacleDetected = false;

    for (char commandChar : command)
    {
        Command cmd = ConvertCharToCommand(commandChar);
        switch (cmd)
        {
        case MOVE_FORWARD:
            if (!RoverMovement(response, 1)) {
                obstacleDetected = true;
            }
            break;

        case MOVE_BACKWARD:
            if (!RoverMovement(response, -1)) {
                obstacleDetected = true;
            }
            break;

        case TURN_LEFT:
            orientation = RotationAntiHoraire(orientation);
            break;

        case TURN_RIGHT:
            orientation = RotationHoraire(orientation);
            break;

        default:
            break;
        }

        if (obstacleDetected)
        {
            break;
        }
    }

    // Display discovered tiles
    for (const auto& tile : response.tilesDiscovered) {
        std::cout << "Discovered tile at (" << tile.x << ", " << tile.y << "): " << tile.type << std::endl;
    }

    response.roverX = positionX;
    response.roverY = positionY;
    response.orientation = orientation;

    return response;
}

void Rover::InitializeRoverPosition() 
{
    Tile** map = planet.getMap();
    int width = planet.getWidth();
    int height = planet.getHeight();

    int roverX = -1, roverY = -1;
    for (int x = 0; x < width && roverX == -1; ++x) {
        for (int y = 0; y < height; ++y) {
            if (map[x][y].type == ROVER) {
                roverX = x;
                roverY = y;
                break;
            }
        }
    }

    positionX = roverX;
    positionY = roverY;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 3);
    Orientation randomOrient = static_cast<Orientation>(dist(gen));
    orientation = randomOrient;
}

void Rover::Main()
{
    // First message to mission control
    RoverPacket initial;
    initial.roverX = positionX;
    initial.roverY = positionY;
    initial.orientation = orientation;
    initial.planetWidth = planet.getWidth();
    initial.planetHeight = planet.getHeight();
    initial.tilesDiscovered.push_back({positionX, positionY, "ROVER"});

    Packet firstPacket;
    firstPacket.setRoverPacket(initial);

    if (!client.Send(firstPacket)) {
        std::cerr << "Send failed" << std::endl;
    }

    while (true) 
    {
        Packet commandPacket;
        if (!client.Receive(commandPacket)) {
            std::cerr << "Connection failed, unable to receive command." << std::endl;
            break;
        }

        MissionControlPacket missionControlPacket = commandPacket.getMissionControlPacket();
        if (missionControlPacket.finished) {
            std::cout << "Mission complete signal received. Shutting down Rover." << std::endl;
            break;
        }
        RoverPacket response = ExecuteCommand(missionControlPacket.listInstructions);

        Packet responsePacket;
        responsePacket.setRoverPacket(response);

        if (!client.Send(responsePacket)) {
            std::cerr << "Send failed" << std::endl;
            break;
        }
    }

    // UnixSocket is RAII-managed; do not call destructor explicitly.

}
