#include <iostream>
#include <string>

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
    if (firstOrientation == NORTH) { return EAST; }
    else if (firstOrientation == EAST) { return SOUTH; }
    else if (firstOrientation == SOUTH) { return WEST; }
    else if (firstOrientation == WEST) { return NORTH; }
}

Orientation RotationAntiHoraire(Orientation firstOrientation)
{
    if (firstOrientation == NORTH) { return WEST; }
    else if (firstOrientation == EAST) { return NORTH; }
    else if (firstOrientation == SOUTH) { return EAST; }
    else if (firstOrientation == WEST) { return SOUTH; }
}

Response Rover::ExecuteCommand(const string &command, Rover &rover)
{
    Response response;

    for (char commandChar : command) {
        switch(commandChar) {
            case MOVE_FORWARD:
              
                if (rover.getOrientation() == NORTH) {
                    rover.setPositionY(rover.getPositionY() + 1);
                } else if (rover.getOrientation() == EAST) {
                    rover.setPositionX(rover.getPositionX() + 1);
                } else if (rover.getOrientation() == SOUTH) {
                    rover.setPositionY(rover.getPositionY() - 1);
                } else if (rover.getOrientation() == WEST) {
                    rover.setPositionX(rover.getPositionX() - 1);
                }
                break;
            
            case MOVE_BACKWARD:
                if (rover.getOrientation() == NORTH) {
                    rover.setPositionY(rover.getPositionY() - 1);
                } else if (rover.getOrientation() == EAST) {
                    rover.setPositionX(rover.getPositionX() - 1);
                } else if (rover.getOrientation() == SOUTH) {
                    rover.setPositionY(rover.getPositionY() + 1);
                } else if (rover.getOrientation() == WEST) {
                    rover.setPositionX(rover.getPositionX() + 1);
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

    }

    response.positionX = rover.getPositionX();
    response.positionY = rover.getPositionY();
    // response.obstacle = false;
    response.orientation = to_string(rover.getOrientation());

    return response;
}

int main()
{
    Rover rover(60, 10, NORTH);
    cout << rover.getPositionX() << ", " << rover.getPositionY() << ", " << rover.getOrientation() << endl;
    return 0;
}