#include <gtest/gtest.h>
#include "planet.hpp"

// =========================================================
// Les classes qui servent de Wrapper pour utiliser les fonctions privates
// =========================================================

class PlanetUnitTest : public ::testing::Test {
protected:
    Planet p_default{10, 20, false}; 
    Planet p_mc{5, 5, true};

    bool CallIsFreeTile(Planet& planet, int x, int y) const {
        return planet.IsFreeTile(x, y); 
    }
};

class PlanetClassTest : public ::testing::Test {
protected:
    Planet p_large{10, 10, false};
    Planet p_mc_only{5, 5, true};
};

class PlanetIntegrationTest : public ::testing::Test {
protected:
    Planet p_integration{10, 10, false};
    
    bool CallIsFreeTile(Planet& planet, int x, int y) const {
        return planet.IsFreeTile(x, y); 
    }
};


// =========================================================
// 2. Tests Unitaires 
// =========================================================

//Taille W
TEST_F(PlanetUnitTest, GetWidth) {
    EXPECT_EQ(p_default.getWidth(), 10);
}

//Taille H
TEST_F(PlanetUnitTest, GetHeight) {
    EXPECT_EQ(p_default.getHeight(), 20);
}

//Case libre 
TEST_F(PlanetUnitTest, IsFreeTileTrue) {
    Tile** map = p_mc.getMap();
    
    // Modification de l'état (nécessaire pour le test)
    map[2][2].type = EMPTY;
    
    EXPECT_TRUE(CallIsFreeTile(p_mc, 2, 2));
}

//Case pas libre 
TEST_F(PlanetUnitTest, IsFreeTileFalse) {
    Tile** map = p_mc.getMap();
    
    map[1][1].type = OBSTACLE;
    
    EXPECT_FALSE(CallIsFreeTile(p_mc, 1, 1));
}

// =========================================================
// 3. Tests de Classe
// =========================================================

//Si elle est pas null
TEST_F(PlanetClassTest, MapIsNotNull) {
    EXPECT_NE(p_large.getMap(), nullptr);
}

//La bonne taille
TEST_F(PlanetClassTest, MapHasCorrectSize) {
    Tile** map = p_large.getMap();

    EXPECT_NE(map[0], nullptr);
    EXPECT_NE(map[p_large.getWidth() - 1], nullptr);
}

//Si y'a un rover sur la map
TEST_F(PlanetClassTest, HasOneRover) {
    Tile** map = p_large.getMap();

    int roverCount = 0;
    int width = p_large.getWidth();
    int height = p_large.getHeight();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (map[i][j].type == ROVER) {
                roverCount++;
            }
        }
    }
    EXPECT_EQ(roverCount, 1);
}

//Verifie si la map de MC est bien inconnu avant exploration
TEST_F(PlanetClassTest, MissionControlHasOnlyUnknown) {
    Tile** map = p_mc_only.getMap();

    for (int i = 0; i < p_mc_only.getWidth(); i++) {
        for (int j = 0; j < p_mc_only.getHeight(); j++) {
            EXPECT_EQ(map[i][j].type, UNKNOWN);
        }
    }
}

// =========================================================
// 4. Test intégration 
// =========================================================

// Verifie que la case du rover 
TEST_F(PlanetIntegrationTest, RoverTileIsNotFree) {
    Tile** map = p_integration.getMap();

    int roverX = -1, roverY = -1;

    for (int i = 0; i < p_integration.getWidth(); i++) {
        for (int j = 0; j < p_integration.getHeight(); j++) {
            if (map[i][j].type == ROVER) {
                roverX = i;
                roverY = j;
            }
        }
    }
    ASSERT_NE(roverX, -1);
    ASSERT_NE(roverY, -1);

    EXPECT_FALSE(CallIsFreeTile(p_integration, roverX, roverY));
}

//Verifie que la map a des obsacles
TEST_F(PlanetIntegrationTest, HasObstacles) {
    Tile** map = p_integration.getMap();

    int obstacleCount = 0;

    for (int i = 0; i < p_integration.getWidth(); i++) {
        for (int j = 0; j < p_integration.getHeight(); j++) {
            if (map[i][j].type == OBSTACLE) {
                obstacleCount++;
            }
        }
    }
     std::cout << "Total Obstacles found: " << obstacleCount << std::endl;
    EXPECT_GT(obstacleCount, 1);
}