#include <gtest/gtest.h>
#include "mission_control.hpp"
#include "planet.hpp"

// =========================================================
// Les classes qui servent de Wrapper pour utiliser les fonctions privates
// =========================================================

class MissionControlUnitTest : public ::testing::Test {
protected:
    MissionControl mc{8000}; 

    bool CallIsValidCommand(const string& command) {
        return mc.IsValidCommand(command);
    }

    ObjectType CallStringToObjectType(const string& typeStr) {
        return mc.StringToObjectType(typeStr);
    }

    void CallUpdateMap(Tile** map, int x, int y, ObjectType type) {
        mc.UpdateMapWithDiscoveredTiles(map, x, y, type);
    }
};

class MissionControlClassTest : public ::testing::Test {
protected:
    MissionControl mc{8000};

    bool CallHasUnknownTiles(Tile** map, int width, int height) const {
        return mc.HasUnknownTiles(map, width, height);
    }
    
    void CallDisplayMap(int width, int height, Tile** map, Orientation orientation) const {
        mc.DisplayMap(width, height, map, orientation);
    }
};

class MissionControlIntegrationTest : public ::testing::Test {
protected:
    MissionControl mc{8000};

    void CallUpdateMap(Tile** map, int x, int y, ObjectType type) {
        mc.UpdateMapWithDiscoveredTiles(map, x, y, type);
    }
};


// =========================================================
// 2. Tests Unitaires 
// =========================================================

TEST_F(MissionControlUnitTest, IsValidCommandTest) {
    EXPECT_TRUE(CallIsValidCommand("F"));
    EXPECT_TRUE(CallIsValidCommand("B"));
    EXPECT_TRUE(CallIsValidCommand("L"));
    EXPECT_TRUE(CallIsValidCommand("R"));
    EXPECT_TRUE(CallIsValidCommand("FLR")); 
}

TEST_F(MissionControlUnitTest, IsInvalidCommandTest) {
    EXPECT_FALSE(CallIsValidCommand("X"));
    EXPECT_FALSE(CallIsValidCommand("Forward")); 
    EXPECT_FALSE(CallIsValidCommand("F B")); 
}

TEST_F(MissionControlUnitTest, StringToObjectTypeConversion) {
    EXPECT_EQ(CallStringToObjectType("ROVER"), ROVER);
    EXPECT_EQ(CallStringToObjectType("OBSTACLE"), OBSTACLE);
    EXPECT_EQ(CallStringToObjectType("EMPTY"), EMPTY);
    EXPECT_EQ(CallStringToObjectType("UNKNOWN"), UNKNOWN);
    EXPECT_EQ(CallStringToObjectType("Alien"), UNKNOWN);
}

TEST_F(MissionControlUnitTest, UpdateMapWithDiscoveredTilesTest) {
    Planet p(5, 5, true); 
    Tile** map = p.getMap();

    EXPECT_EQ(map[2][2].type, UNKNOWN);

    CallUpdateMap(map, 2, 2, OBSTACLE);

    EXPECT_EQ(map[2][2].type, OBSTACLE);
}


// =========================================================
// 3. Tests de Classe 
// =========================================================

TEST_F(MissionControlClassTest, HasUnknownTilesTrueInitially) {
    Planet p(5, 5, true); // Map vide
    EXPECT_TRUE(CallHasUnknownTiles(p.getMap(), 5, 5));
}

TEST_F(MissionControlClassTest, HasUnknownTilesFalseWhenFull) {
    Planet p(2, 2, true);
    Tile** map = p.getMap();

    map[0][0].type = EMPTY;
    map[0][1].type = EMPTY;
    map[1][0].type = OBSTACLE;
    map[1][1].type = ROVER;

    EXPECT_FALSE(CallHasUnknownTiles(map, 2, 2));
}

TEST_F(MissionControlClassTest, DisplayMapRunsWithoutCrash) {
    Planet p(3, 3);
    EXPECT_NO_THROW(CallDisplayMap(3, 3, p.getMap(), NORTH));
}


// =========================================================
// 4. Tests Intégration 
// =========================================================

TEST_F(MissionControlIntegrationTest, UpdateMapLogic) {
    Planet p(5, 5, true);
    Tile** map = p.getMap();

    CallUpdateMap(map, 1, 1, ROVER);
    
    EXPECT_EQ(map[1][1].type, ROVER);
}