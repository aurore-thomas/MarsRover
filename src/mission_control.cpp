#include <iostream>

#include "../include/mission_control.hpp"
#include "../include/planet.hpp"
#include "../include/console.hpp"

using namespace std;

int main() {
    Planet planet(10, 10); // will be send by rover

    Console console;
    console.displayMap(planet.getWidth(), planet.getHeight(), planet.getMap(), NORTH);
    

    return 0;
}