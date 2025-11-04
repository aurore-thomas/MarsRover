#include <cstring>
#include <iostream>

#include "rover.hpp"
#include <random>

using namespace std;

Rover::Rover(Planet &planet) 
{
    InitializeRoverPosition(planet);
}

Orientation Rover::getOrientation() const
{
    return orientation;
}
void Rover::setOrientation(Orientation newOrientation)
{
    orientation = newOrientation;
}
int Rover::getPositionX() const
{
    return positionX;
}
void Rover::setPositionX(const int x)
{
    positionX = x;
}
int Rover::getPositionY() const
{
    return positionY;
}
void Rover::setPositionY(const int y)
{
    positionY = y;
}

bool LaunchClient(UnixSocket &client, const unsigned short port) 
{
    if (!client.Init()) {
        std::cerr << "Client socket init failed" << std::endl;
        return false;
    }

    if (!client.Create()) {
        std::cerr << "Client socket create failed" << std::endl;
        return false;
    }

    if (!client.Connect("127.0.0.1", port)) {
        std::cerr << "Connect failed to 127.0.0.1:" << port << std::endl;
        return false;
    }

    return true;
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

bool Rover::RoverMovement(Rover &rover, Planet &planet, Packet &response, int multiplicator)
{
    if (rover.getOrientation() == NORTH)
    {
        if (planet.IsFreeTile(rover.getPositionX(), rover.getPositionY() + 1 * multiplicator))
        {
            rover.setPositionY(rover.getPositionY() + 1 * multiplicator);
            response.addTileDiscovered(rover.getPositionX(), rover.getPositionY(), "EMPTY");
        }
        else
        {
            response.addTileDiscovered(rover.getPositionX(), rover.getPositionY() + 1 * multiplicator, "OBSTACLE");
            return false;
        }
    }
    else if (rover.getOrientation() == EAST)
    {
        if (planet.IsFreeTile(rover.getPositionX() + 1 * multiplicator, rover.getPositionY()))
        {
            rover.setPositionX(rover.getPositionX() + 1 * multiplicator);
            response.addTileDiscovered(rover.getPositionX(), rover.getPositionY(), "EMPTY");
        }
        else
        {
            response.addTileDiscovered(rover.getPositionX() + 1 * multiplicator, rover.getPositionY(), "OBSTACLE");
            return false;
        }
    }
    else if (rover.getOrientation() == SOUTH)
    {
        if (planet.IsFreeTile(rover.getPositionX(), rover.getPositionY() - 1 * multiplicator))
        {
            rover.setPositionY(rover.getPositionY() - 1 * multiplicator);
            response.addTileDiscovered(rover.getPositionX(), rover.getPositionY(), "EMPTY");
        }
        else
        {
            response.addTileDiscovered(rover.getPositionX(), rover.getPositionY() - 1 * multiplicator, "OBSTACLE");
            return false;
        }
    }
    else if (rover.getOrientation() == WEST)
    {
        if (planet.IsFreeTile(rover.getPositionX() - 1 * multiplicator, rover.getPositionY()))
        {
            rover.setPositionX(rover.getPositionX() - 1 * multiplicator);
            response.addTileDiscovered(rover.getPositionX(), rover.getPositionY(), "EMPTY");
        }
        else
        {
            response.addTileDiscovered(rover.getPositionX() - 1 * multiplicator, rover.getPositionY(), "OBSTACLE");
            return false;
        }
    }
    return true;
}

Command ConvertCharToCommand(char commandChar)
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

Packet Rover::ExecuteCommand(const string &command, Rover &rover, Planet &planet)
{
    Packet response;
    bool obstacleDetected = false;

    for (char commandChar : command)
    {
        Command cmd = ConvertCharToCommand(commandChar);
        switch (cmd)
        {
        case MOVE_FORWARD:
            if (!RoverMovement(rover, planet, response, 1)) {
                obstacleDetected = true;
            }
            break;

        case MOVE_BACKWARD:
            if (!RoverMovement(rover, planet, response, -1)) {
                obstacleDetected = true;
            }
            break;

        case TURN_LEFT:
            rover.setOrientation(RotationAntiHoraire(rover.getOrientation()));
            break;

        case TURN_RIGHT:
            rover.setOrientation(RotationHoraire(rover.getOrientation()));
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
    for (const auto& tile : response.getTilesDiscovered()) {
        std::cout << "Discovered tile at (" << tile.x << ", " << tile.y << "): " << tile.type << std::endl;
    }

    response.setPacketOrientation(rover.getOrientation());

    return response;
}

void Rover::InitializeRoverPosition(Planet &planet) 
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

    setPositionX(roverX);
    setPositionY(roverY);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 3);
    Orientation randomOrient = static_cast<Orientation>(dist(gen));
    setOrientation(randomOrient);
}


int main()
{
    // Todo: configure port and planet size via command line arguments
    const unsigned short port = 8080;
    int planetWidth = 15;
    int planetHeight = 15;

    UnixSocket client;
    Planet planet(planetWidth, planetHeight);
    planet.setMap(planet.createMap(planetWidth, planetHeight));

    Rover rover(planet);

    if (!LaunchClient(client, port)) {
        return 1;
    }

    // First message to mission control
    Packet firstPacket; 
    firstPacket.setPacketOrientation(rover.getOrientation());
    firstPacket.setPacketPlanetHeight(planet.getHeight());
    firstPacket.setPacketPlanetWidth(planet.getWidth());
    firstPacket.addTileDiscovered(rover.getPositionX(), rover.getPositionY(), "ROVER");

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

        if (commandPacket.isFinished()) {
            std::cout << "Mission complete signal received. Shutting down Rover." << std::endl;
            break;
        }

        cout << "Received command : " << commandPacket.getListInstructions() << std::endl;

        Packet response = rover.ExecuteCommand(commandPacket.getListInstructions(), rover, planet);
        cout << "Nb of tiles discovered to send: " << response.getTilesDiscovered().size() << std::endl;

        if (!client.Send(response)) {
            std::cerr << "Send failed" << std::endl;
            break;
        }
    }

    client.Close();
    
    return 0;
}