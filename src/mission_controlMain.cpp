#include <iostream>
#include <cstring>

#include "mission_control.hpp"

using namespace std;


int main(int argc, char* argv[])
{    
    if (argc != 2) {
        std::cerr << "Usage: mission_control [port]" << std::endl;
        return 1;
    }
    unsigned short port = std::stoi(argv[1]);

    MissionControl missionControl = MissionControl(port);
    missionControl.Main();

    return 0;
}
