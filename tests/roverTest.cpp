#include <gtest/gtest.h>
#include "planet.hpp"
#include "rover.hpp"

//========================================================================
// Classe helper pour tests
class RoverTestHelper : public Rover {
public:
    // Hérite du constructeur de Rover
    using Rover::Rover;

    // Accès aux membres protected
    int getX() const { return positionX; }
    int getY() const { return positionY; }

    void setPosition(int x, int y) { positionX = x; positionY = y; }

    Orientation getOrientation() const { return orientation; }
    void setOrientation(Orientation o) { orientation = o; }
};
//========================================================================

//Test unitaire

//Si rover est bien dans la planet avec el modulo
TEST(RoverUnitTest, ModuloPositive)
{
    Planet p(5,5);
    Rover r(p, 0, "127.0.0.1");

    EXPECT_EQ(r.Modulo(6,5), 1);
    EXPECT_EQ(r.Modulo(-1,5), 4);
    EXPECT_EQ(r.Modulo(5,5), 0);
}

//Si il est assez grand pour conaitre sa gauche et sa droite
TEST(RoverUnitTest, ClockwiseRotation)
{
    Planet p(5,5);
    Rover r(p, 0, "127.0.0.1");

    EXPECT_EQ(r.RotationHoraire(NORTH), EAST);
    EXPECT_EQ(r.RotationHoraire(EAST), SOUTH);
    EXPECT_EQ(r.RotationHoraire(SOUTH), WEST);
    EXPECT_EQ(r.RotationHoraire(WEST), NORTH);
}

//Pareil
TEST(RoverUnitTest, CounterClockwiseRotation)
{
    Planet p(5,5);
    Rover r(p, 0, "127.0.0.1");

    EXPECT_EQ(r.RotationAntiHoraire(NORTH), WEST);
    EXPECT_EQ(r.RotationAntiHoraire(WEST), SOUTH);
    EXPECT_EQ(r.RotationAntiHoraire(SOUTH), EAST);
    EXPECT_EQ(r.RotationAntiHoraire(EAST), NORTH);
}

//Test de Classe

TEST(RoverClassTest, RoverHasValidInitialPosition)
{
    Planet planet(10,10);
    Rover rover(planet, 0, "127.0.0.1");

    EXPECT_GE(rover.getX(), 0);
    EXPECT_GE(rover.getY(), 0);

    EXPECT_LT(rover.getX(), planet.getWidth());
    EXPECT_LT(rover.getY(), planet.getHeight());
}

TEST(RoverClassTest, RoverGivesCorrectPacketAfterMove)
{
    Planet planet(10,10);
    Rover rover(planet, 0, "127.0.0.1");

    RoverPacket response = rover.ExecuteCommand("F");

    EXPECT_GE(response.roverX, 0);
    EXPECT_GE(response.roverY, 0);
    EXPECT_LT(response.roverX, planet.getWidth());
    EXPECT_LT(response.roverY, planet.getHeight());

    EXPECT_NE(response.tilesDiscovered.size(), 0);
}

//Test intégration

TEST(RoverIntegrationTest, RoverWrapsAroundPlanet)
{
    Planet planet(5,5);
    Rover rover(planet, 0, "127.0.0.1");

    // On force position manuellement près du bord
    rover.setPosition(4,4);
    rover.setOrientation(NORTH);

    RoverPacket response = rover.ExecuteCommand("F");

    EXPECT_EQ(response.roverY, 0);
}

TEST(RoverIntegrationTest, RoverDetectsObstacle)
{
    Planet planet(5,5);

    // On force un obstacle à la main
    planet.getMap()[2][2].type = OBSTACLE;

    Rover rover(planet, 0, "127.0.0.1");
    rover.setPosition(2,1);
    rover.setOrientation(NORTH);

    RoverPacket response = rover.ExecuteCommand("F");

    ASSERT_EQ(response.tilesDiscovered.size(), 1);
    EXPECT_EQ(response.tilesDiscovered[0].type, "OBSTACLE");
    EXPECT_EQ(response.roverY, 1); 
}

TEST(RoverIntegrationTest, RoverStopsOnObstacleInSequence)
{
    Planet planet(5,5);
    planet.getMap()[3][3].type = OBSTACLE;

    Rover rover(planet, 0, "127.0.0.1");
    rover.setPosition(3,2);
    rover.setOrientation(NORTH);

    RoverPacket response = rover.ExecuteCommand("FFF");

    EXPECT_EQ(response.roverX, 3);
    EXPECT_EQ(response.roverY, 2);
}

TEST(RoverIntegrationTest, RoverComplexPath)
{
    Planet planet(10,10);
    Rover rover(planet, 0, "127.0.0.1");

    rover.setPosition(3,3);
    rover.setOrientation(NORTH);

    RoverPacket response = rover.ExecuteCommand("FFRFF");

    EXPECT_TRUE(response.roverX >= 0);
    EXPECT_TRUE(response.roverY >= 0);
    EXPECT_TRUE(response.roverX < planet.getWidth());
    EXPECT_TRUE(response.roverY < planet.getHeight());
}
