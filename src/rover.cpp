#include <cstring>
#include <iostream>

#include "rover.hpp"
#include "planet.hpp"

using namespace std;

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

int main()
{
//     int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    
//     sockaddr_in serverAddress;
//     serverAddress.sin_family = AF_INET;
//     serverAddress.sin_port = htons(8080);
//     serverAddress.sin_addr.s_addr = INADDR_ANY;

//     connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

//     const char *message = "Hello, server !";
//     send(clientSocket, message, strlen(message), 0);
//     char buffer[1024] = {0};
    
//     recv(clientSocket, buffer, sizeof(buffer), 0);
//     cout << "Message from server: " << buffer << endl;
    
// #ifdef _UNIX__
//     close(clientSocket);
// #endif
    return 0;
}