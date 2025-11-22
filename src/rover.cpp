#include <iostream>
#include <random>

#include "rover.hpp"

using namespace std;

Rover::Rover(Planet &planet, UnixSocket &client, const unsigned short port, string address) 
{
    InitializeRoverPosition(planet);
    if (!LaunchClient(client, port, address)) {
        std::cerr << "Connection to Mission Control failed" << std::endl;
    }
    else {
        std::cout << "Connected to Mission Control at " << address << ":" << port << std::endl;
    }
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

int modulo(int a, int b){
    return ((a % b) + b ) % b;
}

bool RoverMovement(Rover &rover, Planet &planet, RoverPacket &response, int multiplicator)
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
            RoverPacket::TileDiscovered tile{x, y, "EMPTY"};
            response.tilesDiscovered.push_back(std::move(tile));
        }
        else
        {
            RoverPacket::TileDiscovered tile{x, modulo(y + 1 * multiplicator,h), "OBSTACLE"};
            response.tilesDiscovered.push_back(std::move(tile));
            return false;
        }
    }
    else if (rover.getOrientation() == EAST)
    {
        if (planet.IsFreeTile(modulo(x + 1 * multiplicator,w), y))
        {
            rover.setPositionX(modulo(x + 1 * multiplicator,w));
            RoverPacket::TileDiscovered tile{x, y, "EMPTY"};
            response.tilesDiscovered.push_back(std::move(tile));
        }
        else
        {
            RoverPacket::TileDiscovered tile{modulo(x + 1 * multiplicator,w), y, "OBSTACLE"};
            response.tilesDiscovered.push_back(std::move(tile));
            return false;
        }
    }
    else if (rover.getOrientation() == SOUTH)
    {
        if (planet.IsFreeTile(x, modulo(y - 1 * multiplicator,h)))
        {
            rover.setPositionY(modulo(y - 1 * multiplicator,h));
            RoverPacket::TileDiscovered tile{x, y, "EMPTY"};
            response.tilesDiscovered.push_back(std::move(tile));
        }
        else
        {
            RoverPacket::TileDiscovered tile{x, modulo(y - 1 * multiplicator,h), "OBSTACLE"};
            response.tilesDiscovered.push_back(std::move(tile));
            return false;
        }
    }
    else if (rover.getOrientation() == WEST)
    {
        if (planet.IsFreeTile(modulo(x - 1 * multiplicator,w), y))
        {
            rover.setPositionX(modulo(x - 1 * multiplicator,w));
            RoverPacket::TileDiscovered tile{x, y, "EMPTY"};
            response.tilesDiscovered.push_back(std::move(tile));
        }
        else
        {
            RoverPacket::TileDiscovered tile{modulo(x - 1 * multiplicator,w), y, "OBSTACLE"};
            response.tilesDiscovered.push_back(std::move(tile));
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

RoverPacket ExecuteCommand(const string &command, Rover &rover, Planet &planet)
{
    RoverPacket response;
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
    for (const auto& tile : response.tilesDiscovered) {
        std::cout << "Discovered tile at (" << tile.x << ", " << tile.y << "): " << tile.type << std::endl;
    }

    response.roverX = rover.getPositionX();
    response.roverY = rover.getPositionY();
    response.orientation = rover.getOrientation();

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
    RoverPacket initial;
    initial.roverX = rover.getPositionX();
    initial.roverY = rover.getPositionY();
    initial.orientation = rover.getOrientation();
    initial.planetWidth = planet.getWidth();
    initial.planetHeight = planet.getHeight();
    initial.tilesDiscovered.push_back({rover.getPositionX(), rover.getPositionY(), "ROVER"});

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
        RoverPacket response = ExecuteCommand(missionControlPacket.listInstructions, rover, planet);

        Packet responsePacket;
        responsePacket.setRoverPacket(response);

        if (!client.Send(responsePacket)) {
            std::cerr << "Send failed" << std::endl;
            break;
        }
    }

    client.~UnixSocket();
    return 0;
}