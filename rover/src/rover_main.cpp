#include <iostream>
#include <random>

#include "rover.hpp"

using namespace std;

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

    Planet planet(planetWidth, planetHeight);

    Rover rover(planet, port, address);
    rover.Main();
    
    return 0;
}