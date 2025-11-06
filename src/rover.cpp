#include <cstring>
#include <iostream>

#include "rover.hpp"

#include <random>
#include "planet.hpp"
#include "unix_socket.hpp"

using namespace std;

Rover::Rover(Planet &planet, UnixSocket &client, const unsigned short port, string address) 
{
    InitializeRoverPosition(planet);
    LaunchClient(client, port, address);
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

bool Rover::LaunchClient(UnixSocket &client, const unsigned short port, string address) 
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

/*

int modulo(int a, int b){
    return ((a % b) + b ) % b
}

bool Rover::RoverMovement(Rover &rover, Planet &planet, Packet &response, int multiplicator)
{
    int x = rover.getPositionX();
    int y = rover.getPositionY();
    int w = planet.getWidth();
    int h = planet.getHeight();


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
        //if (planet.IsFreeTile(((rover.getPositionX() + 1 * multiplicator) % planet.getWidth() + planet.getWidth()) % planet.getWidth(), rover.getPositionY()))        
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
*/// SI je me sent de la faire opti


int modulo(int a, int b){
    return ((a % b) + b ) % b;
}

bool Rover::RoverMovement(Rover &rover, Planet &planet, Packet &response, int multiplicator)
{
    int x = rover.getPositionX();
    int y = rover.getPositionY();
    int w = planet.getWidth();
    int h = planet.getHeight();


    if (rover.getOrientation() == NORTH)
    {
        if (planet.IsFreeTile(x, modulo(y + 1 * multiplicator,h)))
        {
            rover.setPositionY(modulo(y + 1 * multiplicator,h));
            response.addTileDiscovered(x, y, "EMPTY");
        }
        else
        {
            response.addTileDiscovered(x, modulo(y + 1 * multiplicator,h), "OBSTACLE");
            return false;
        }
    }
    else if (rover.getOrientation() == EAST)
    {
        //if (planet.IsFreeTile(((modulo(x + 1 * multiplicator,h)) % planet.getWidth() + planet.getWidth()) % planet.getWidth(), y))        
        if (planet.IsFreeTile(modulo(x + 1 * multiplicator,w), y))
        {
            rover.setPositionX(modulo(x + 1 * multiplicator,w));
            response.addTileDiscovered(x, y, "EMPTY");
        }
        else
        {
            response.addTileDiscovered(modulo(x + 1 * multiplicator,w), y, "OBSTACLE");
            return false;
        }
    }
    else if (rover.getOrientation() == SOUTH)
    {
        if (planet.IsFreeTile(x, modulo(y - 1 * multiplicator,h)))
        {
            rover.setPositionY(modulo(y - 1 * multiplicator,h));
            response.addTileDiscovered(x, y, "EMPTY");
        }
        else
        {
            response.addTileDiscovered(x, modulo(y - 1 * multiplicator,h), "OBSTACLE");
            return false;
        }
    }
    else if (rover.getOrientation() == WEST)
    {
        if (planet.IsFreeTile(modulo(x - 1 * multiplicator,w), y))
        {
            rover.setPositionX(modulo(x - 1 * multiplicator,w));
            response.addTileDiscovered(x, y, "EMPTY");
        }
        else
        {
            response.addTileDiscovered(modulo(x - 1 * multiplicator,w), y, "OBSTACLE");
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


int main(int argc, char* argv[])
{
    if (argc != 5) {
        std::cerr << "Usage: [port] [address] [planet width] [planet height]" << std::endl;
        return 1;
    }

    const unsigned short port = std::stoi(argv[1]);
    const string address = argv[2];

    int planetWidth = std::stoi(argv[3]);
    int planetHeight = std::stoi(argv[4]);

    UnixSocket client = UnixSocket();
    Planet planet(planetWidth, planetHeight);
    planet.setMap(planet.createMap(planetWidth, planetHeight));

    Rover rover(planet, client, port, address);

    // First message to mission control
    Packet firstPacket; 
    firstPacket.setPacketOrientation(rover.getOrientation());
    firstPacket.setPacketPlanetHeight(planet.getHeight());
    firstPacket.setPacketPlanetWidth(planet.getWidth());
    firstPacket.addTileDiscovered(rover.getPositionX(), rover.getPositionY(), "ROVER");

    cout << "Sending initial rover position: (" << rover.getPositionX() << ", " << rover.getPositionY() << ") with orientation " << rover.getOrientation() << std::endl;

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
        response.setPacketRoverX(rover.getPositionX());
        response.setPacketRoverY(rover.getPositionY());

        if (!client.Send(response)) {
            std::cerr << "Send failed" << std::endl;
            break;
        }
    }

    client.~UnixSocket();
    return 0;
}