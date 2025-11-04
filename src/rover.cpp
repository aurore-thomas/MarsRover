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

void Rover::RoverMovement(Rover &rover, Planet &planet, Packet &response, int multiplicator)
{
    if (rover.getOrientation() == NORTH)
    {
        if (planet.IsFreeTile(rover.getPositionX(), rover.getPositionY() + 1 * multiplicator))
        {
            rover.setPositionY(rover.getPositionY() + 1 * multiplicator);
        }
        else
        {
            response.setPacketObstacle(true);
        }
    }
    else if (rover.getOrientation() == EAST)
    {
        if (planet.IsFreeTile(rover.getPositionX() + 1 * multiplicator, rover.getPositionY()))
        {
            rover.setPositionX(rover.getPositionX() + 1 * multiplicator);
        }
        else
        {
            response.setPacketObstacle(true);
        }
    }
    else if (rover.getOrientation() == SOUTH)
    {
        if (planet.IsFreeTile(rover.getPositionX(), rover.getPositionY() - 1 * multiplicator))
        {
            rover.setPositionY(rover.getPositionY() - 1 * multiplicator);
        }
        else
        {
            response.setPacketObstacle(true);
        }
    }
    else if (rover.getOrientation() == WEST)
    {
        if (planet.IsFreeTile(rover.getPositionX() - 1 * multiplicator, rover.getPositionY()))
        {
            rover.setPositionX(rover.getPositionX() - 1 * multiplicator);
        }
        else
        {
            response.setPacketObstacle(true);
        }
    }
}

Packet Rover::ExecuteCommand(const string &command, Rover &rover, Planet &planet)
{
    Packet response;
    response.setPacketObstacle(false);

    for (char commandChar : command)
    {
        switch (commandChar)
        {
        case MOVE_FORWARD:
            RoverMovement(rover, planet, response, 1);
            break;

        case MOVE_BACKWARD:
            RoverMovement(rover, planet, response, -1);
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

        if (response.getPacketObstacle())
        {
            break;
        }
    }

    response.setPacketPositionX(rover.getPositionX());
    response.setPacketPositionY(rover.getPositionY());
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

    if (roverX == -1) {
        std::cerr << "No rover found on planet map" << std::endl;
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

    cout << "planet width: " << planet.getWidth() << endl;
    cout << "planet height: " << planet.getHeight() << endl;

    Rover rover(planet);

    if (!LaunchClient(client, port)) {
        return 1;
    }

    bool obstacleDetected = false;

    // First message to mission control
    Packet packet; 
    packet.setPacketOrientation(rover.getOrientation());
    packet.setPacketPositionX(rover.getPositionX());
    packet.setPacketPositionY(rover.getPositionY());
    packet.setPacketObstacle(obstacleDetected);
    packet.setPacketPlanetHeight(planet.getHeight());
    packet.setPacketPlanetWidth(planet.getWidth());

    if (!client.Send(packet)) {
        std::cerr << "Send failed" << std::endl;
    }

    // while (true) 
    // {
    //     return 0;
    // }

    // Packet response;
    // if (!client.Receive(response)) {
    //     std::cerr << "Receive failed or no data" << std::endl;
    // } else {
    //     std::cout << "Message from server: " << response.getListInstructions() << std::endl;
    // }

    client.Close();
    
    return 0;
}