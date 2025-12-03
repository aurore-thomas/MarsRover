#include <gtest/gtest.h>
#include "planet.hpp"
#include "rover.hpp"


// =========================================================
// Les classes qui servent de Wrapper pour utiliser les fonctions privates
// =========================================================

class RoverUnitTest : public ::testing::Test {
protected:
    Planet p{5, 5};
    Rover r{p, 8000, "127.0.0.1"};

    int CallModulo(int a, int b) {
        return r.Modulo(a, b); 
    }

    Orientation CallRotationHoraire(Orientation o) {
        return r.RotationHoraire(o);
    }

    Orientation CallRotationAntiHoraire(Orientation o) {
        return r.RotationAntiHoraire(o);
    }
};

class RoverClassTest : public ::testing::Test {
protected:
    Planet p{5, 5};
    Rover r{p, 8000, "127.0.0.1"};

public:
    RoverPacket CallExecuteCommand(const string& cmd) {
        return r.ExecuteCommand(cmd);
    }
};

class RoverIntegrationTest : public ::testing::Test {
protected:
    Planet p{5, 5};
    Rover r{p, 8000, "127.0.0.1"};

public:
    RoverPacket CallExecuteCommand(const string& cmd) {
        return r.ExecuteCommand(cmd);
    }
};


// =========================================================
// 2. Tests Unitaires 
// =========================================================

//Si rover est bien dans la planet avec le modulo
TEST_F(RoverUnitTest, ModuloPositive)
{
    EXPECT_EQ(CallModulo(6,5), 1);
    EXPECT_EQ(CallModulo(-1,5), 4);
    EXPECT_EQ(CallModulo(5,5), 0);
}

//Si il est assez grand pour conaitre sa gauche et sa droite
TEST_F(RoverUnitTest, ClockwiseRotation)
{
    EXPECT_EQ(CallRotationHoraire(NORTH), EAST);
    EXPECT_EQ(CallRotationHoraire(EAST), SOUTH);
    EXPECT_EQ(CallRotationHoraire(SOUTH), WEST);
    EXPECT_EQ(CallRotationHoraire(WEST), NORTH);
}

//Pareil
TEST_F(RoverUnitTest, CounterClockwiseRotation)
{

    EXPECT_EQ(CallRotationAntiHoraire(NORTH), WEST);
    EXPECT_EQ(CallRotationAntiHoraire(WEST), SOUTH);
    EXPECT_EQ(CallRotationAntiHoraire(SOUTH), EAST);
    EXPECT_EQ(CallRotationAntiHoraire(EAST), NORTH);
}

// =========================================================
// 3. Tests de Classe
// =========================================================

//Verifie que rover est bien dans la map
TEST_F(RoverClassTest, RoverHasValidInitialPosition)
{

    EXPECT_GE(r.getX(), 0);
    EXPECT_GE(r.getY(), 0);

    EXPECT_LT(r.getX(), p.getWidth());
    EXPECT_LT(r.getY(), p.getHeight());
}

//Verifie que les instructions sont valide
TEST_F(RoverClassTest, RoverGivesCorrectPacketAfterMove)
{
    RoverPacket response = CallExecuteCommand("F");

    EXPECT_GE(response.roverX, 0);
    EXPECT_GE(response.roverY, 0);
    EXPECT_LT(response.roverX, p.getWidth());
    EXPECT_LT(response.roverY, p.getHeight());

    EXPECT_NE(response.tilesDiscovered.size(), 0);
}


// =========================================================
// 4. Test intégration 
// =========================================================

//Verifie que rover execute bien l'action
TEST_F(RoverIntegrationTest, RoverSimpleCommand) 
{

    int startX = r.getX();
    int startY = r.getY();
    
    std::cout << "Initial Position: (" << startX << ", " << startY << "), Orientation: " << r.getOrientation() << std::endl;
    RoverPacket response = CallExecuteCommand("F");
    std::cout << "New Position: (" << r.getX() << ", " << r.getY() << "), Orientation: " << r.getOrientation() << std::endl;
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
