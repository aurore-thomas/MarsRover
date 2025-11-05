#include <cstring>
#include <iostream>

#include "rover.hpp"
#include "planet.hpp"
#include "common/unix_socket.hpp"

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

Rover::Rover(int x, int y, Orientation orientation) 
{
    setPositionX(x);
    setPositionY(y);
    setOrientation(orientation);
}

Orientation RotationHoraire(Orientation firstOrientation)
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

Orientation RotationAntiHoraire(Orientation firstOrientation)
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
            response.addTileDiscovered(rover.getPositionX(), rover.getPositionY(), "EMPTY");
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
            response.addTileDiscovered(rover.getPositionX(), rover.getPositionY(), "EMPTY");
        }
        else
        {
            response.setPacketObstacle(true);
        }
    }
    else if (rover.getOrientation() == SOUTH)
    {
        if (planet.IsFreeTile(rover.getPositionX(),rover.getPositionY() - 1 * multiplicator ))
        {
            rover.setPositionY(rover.getPositionY() - 1 * multiplicator);
            response.addTileDiscovered(rover.getPositionX(), rover.getPositionY(), "EMPTY");
        }
        else
        {
            response.setPacketObstacle(true);
        }
    }
    else if (rover.getOrientation() == WEST)
    {
        if (planet.IsFreeTile(rover.getPositionX() - 1 * multiplicator , rover.getPositionY()))
        {
            rover.setPositionX(rover.getPositionX() - 1 * multiplicator);
            response.addTileDiscovered(rover.getPositionX(), rover.getPositionY(), "EMPTY");
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

int main()
{
    const unsigned short port = 8080;

    UnixSocket client;

    if (!client.Init()) {
        std::cerr << "Client socket init failed" << std::endl;
        return 1;
    }

    if (!client.Create()) {
        std::cerr << "Client socket create failed" << std::endl;
        return 1;
    }

    if (!client.Connect("127.0.0.1", port)) {
        std::cerr << "Connect failed to 127.0.0.1:" << port << std::endl;
        return 1;
    }

    Packet response;
    if (!client.Receive(response)) {
        std::cerr << "Receive failed or no data" << std::endl;
    } else {
        std::cout << "Message from server: " << response.getListInstructions() << std::endl;
    }

    client.Close();
    
    return 0;
}