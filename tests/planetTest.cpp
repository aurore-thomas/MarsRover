#include <gtest/gtest.h>
#include "planet.hpp"

// Test de Unitaire 

//Taille W
TEST(PlanetUnitTest, GetWidth) {
    Planet p(10, 20, false); 
    EXPECT_EQ(p.getWidth(), 10);
}

//Taille H
TEST(PlanetUnitTest, GetHeight) {
    Planet p(10, 20, false);
    EXPECT_EQ(p.getHeight(), 20);
}

//Case libre
TEST(PlanetUnitTest, IsFreeTileTrue) {
    Planet p(5, 5, false);
    Tile** map = p.getMap();

    // On vide une case exprès
    //std::cout << std::endl <<" Avant: " << map[2][2].type << std::endl;
    map[2][2].type = EMPTY;
    //std::cout <<" Apres: " << map[2][2].type << std::endl;
    EXPECT_TRUE(p.IsFreeTile(2, 2));

}

//Case pas libre
TEST(PlanetUnitTest, IsFreeTileFalse) {
    Planet p(5, 5, true);
    Tile** map = p.getMap();
    //std::cout << std::endl <<" Avant " << map[1][1].type << std::endl;
    map[1][1].type = OBSTACLE;
    //std::cout <<" Apres: " << map[1][1].type << std::endl;
    EXPECT_FALSE(p.IsFreeTile(1, 1));
}

//===============================================
// Test de Classe 
//===============================================

TEST(PlanetClassTest, MapIsNotNull) {
    Planet p(10, 10, true);

    EXPECT_NE(p.getMap(), nullptr);
}

TEST(PlanetClassTest, MapHasCorrectSize) {
    Planet p(8, 8, true);
    Tile** map = p.getMap();

    EXPECT_NE(map[0], nullptr);
    EXPECT_NE(map[7], nullptr);
}

//Si y'a un rover sur la map
TEST(PlanetClassTest, HasOneRover) {
    Planet p(10, 10, false); 
    Tile** map = p.getMap();

    int roverCount = 0;
    int width = p.getWidth();
    int height = p.getHeight();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            if (map[i][j].type == ROVER) {
                //std::cout << "Rover found at: " << i << "," << j << std::endl;
                roverCount++;
            }
        }
    }
    //std::cout << "Total Rovers found: " << roverCount << std::endl;
    EXPECT_EQ(roverCount, 1);
}


TEST(PlanetClassTest, MissionControlHasOnlyUnknown) {
    Planet p(5, 5, true);
    Tile** map = p.getMap();

    for (int i = 0; i < p.getWidth(); i++) {
        for (int j = 0; j < p.getHeight(); j++) {
            EXPECT_EQ(map[i][j].type, UNKNOWN);
        }
    }
}

//===============================================
// Test intégration 
//===============================================

TEST(PlanetIntegrationTest, RoverTileIsNotFree) {
    Planet p(10, 10, false);
    Tile** map = p.getMap();

    int roverX = -1, roverY = -1;

    for (int i = 0; i < p.getWidth(); i++) {
        for (int j = 0; j < p.getHeight(); j++) {
            if (map[i][j].type == ROVER) {
                roverX = i;
                roverY = j;
            }
        }
    }
    ASSERT_NE(roverX, -1) ;
    ASSERT_NE(roverY, -1) ;

    EXPECT_FALSE(p.IsFreeTile(roverX, roverY));
}


TEST(PlanetIntegrationTest, HasObstacles) {
    Planet p(10, 10, false);
    Tile** map = p.getMap();

    int obstacleCount = 0;

    for (int i = 0; i < p.getWidth(); i++) {
        for (int j = 0; j < p.getHeight(); j++) {
            if (map[i][j].type == OBSTACLE) {
                obstacleCount++;
            }
        }
    }
    std::cout << "Total Obstacles found: " << obstacleCount << std::endl;    
    EXPECT_GT(obstacleCount, 1);
}
