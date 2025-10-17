#include <cstring>
#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#elif __unix__
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
typedef int SOCKET;
#define INVALID_SOCKET -1
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
#define SOCKET_ERROR -1
#else
#error "Unknown"
#endif

#include "../include/rover.hpp"

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

Rover::Rover(int x, int y, Orientation orientation) : Communication()
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

void Rover::RoverMovement(Rover &rover, Planet &planet, Response &response, int multiplicator)
{
    if (rover.getOrientation() == NORTH)
    {
        if (planet.CaseIsFree(rover.getPositionX(), rover.getPositionY() + 1 * multiplicator))
        {
            rover.setPositionY(rover.getPositionY() + 1 * multiplicator);
        }
        else
        {
            response.obstacle = true;
        }
    }
    else if (rover.getOrientation() == EAST)
    {
        if (planet.CaseIsFree(rover.getPositionX() + 1 * multiplicator, rover.getPositionY()))
        {
            rover.setPositionX(rover.getPositionX() + 1 * multiplicator);
        }
        else
        {
            response.obstacle = true;
        }
    }
    else if (rover.getOrientation() == SOUTH)
    {
        if (planet.CaseIsFree(rover.getPositionX(), rover.getPositionY() - 1 * multiplicator))
        {
            rover.setPositionY(rover.getPositionY() - 1 * multiplicator);
        }
        else
        {
            response.obstacle = true;
        }
    }
    else if (rover.getOrientation() == WEST)
    {
        if (planet.CaseIsFree(rover.getPositionX() - 1 * multiplicator, rover.getPositionY()))
        {
            rover.setPositionX(rover.getPositionX() - 1 * multiplicator);
        }
        else
        {
            response.obstacle = true;
        }
    }
}

Response Rover::ExecuteCommand(const string &command, Rover &rover, Planet &planet)
{
    Response response;
    response.obstacle = false;

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

        if (response.obstacle)
        {
            break;
        }
    }

    response.positionX = rover.getPositionX();
    response.positionY = rover.getPositionY();
    response.orientation = to_string(rover.getOrientation());

    return response;
}

int main()
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    const char *message = "Hello, server !";
    send(clientSocket, message, strlen(message), 0);
    char buffer[1024] = {0};

    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Message from server: " << buffer << endl;

    close(clientSocket);

    return 0;
}