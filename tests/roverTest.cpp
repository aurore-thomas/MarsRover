#include <gtest/gtest.h>
#include "planet.hpp"
#include "rover.hpp"


//Test unitaire
//Si rover est bien dans la planet avec le modulo
TEST(RoverUnitTest, ModuloPositive)
{
    Planet p(5,5);
    Rover r(p, 8000, "127.0.0.1");

    EXPECT_EQ(r.Modulo(6,5), 1);
    EXPECT_EQ(r.Modulo(-1,5), 4);
    EXPECT_EQ(r.Modulo(5,5), 0);
}

//Si il est assez grand pour conaitre sa gauche et sa droite
TEST(RoverUnitTest, ClockwiseRotation)
{
    Planet p(5,5);
    Rover r(p, 8001, "127.0.0.1");

    EXPECT_EQ(r.RotationHoraire(NORTH), EAST);
    EXPECT_EQ(r.RotationHoraire(EAST), SOUTH);
    EXPECT_EQ(r.RotationHoraire(SOUTH), WEST);
    EXPECT_EQ(r.RotationHoraire(WEST), NORTH);
}

//Pareil
TEST(RoverUnitTest, CounterClockwiseRotation)
{
    Planet p(5,5);
    Rover r(p, 8000, "127.0.0.1");

    EXPECT_EQ(r.RotationAntiHoraire(NORTH), WEST);
    EXPECT_EQ(r.RotationAntiHoraire(WEST), SOUTH);
    EXPECT_EQ(r.RotationAntiHoraire(SOUTH), EAST);
    EXPECT_EQ(r.RotationAntiHoraire(EAST), NORTH);
}

//Test de Classe

TEST(RoverClassTest, RoverHasValidInitialPosition)
{
    Planet p(10,10);
    Rover r(p, 8000, "127.0.0.1");

    EXPECT_GE(r.getX(), 0);
    EXPECT_GE(r.getY(), 0);

    EXPECT_LT(r.getX(), p.getWidth());
    EXPECT_LT(r.getY(), p.getHeight());
}

TEST(RoverClassTest, RoverGivesCorrectPacketAfterMove)
{
    Planet p(10,10);
    Rover r(p, 8000, "127.0.0.1");

    RoverPacket response = r.ExecuteCommand("F");

    EXPECT_GE(response.roverX, 0);
    EXPECT_GE(response.roverY, 0);
    EXPECT_LT(response.roverX, p.getWidth());
    EXPECT_LT(response.roverY, p.getHeight());

    EXPECT_NE(response.tilesDiscovered.size(), 0);
}


//Test intégration

TEST(RoverIntegrationTest, RoverSimpleCommand) // ***
{
    Planet p(5,5);
    Rover r(p, 8000, "127.0.0.1");

    int startX = r.getX();
    int startY = r.getY();
    
    //std::cout << "Initial Position: (" << startX << ", " << startY << "), Orientation: " << r.getOrientation() << std::endl;
    RoverPacket response = r.ExecuteCommand("F");
    //std::cout << "New Position: (" << r.getX() << ", " << r.getY() << "), Orientation: " << r.getOrientation() << std::endl;
    if(r.getOrientation() == EAST) {
        EXPECT_TRUE(r.getX() >= startX or r.getX() == 0);
        EXPECT_TRUE(r.getY() == startY);
    }else if(r.getOrientation() == NORTH) {
        EXPECT_TRUE(r.getX() == startX);
        EXPECT_TRUE(r.getY() >= startY or r.getY()== 0);
    } else if(r.getOrientation() == WEST) {
        EXPECT_TRUE(r.getX() <= startX or r.getX() == p.getHeight()-1);
        EXPECT_TRUE(r.getY() == startY);
    } else if(r.getOrientation() == SOUTH) {
        EXPECT_TRUE(r.getX() == startX);
        EXPECT_TRUE(r.getY() <= startY or r.getY() == p.getWidth()-1);
    }
}
