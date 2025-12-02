#include <gtest/gtest.h>
#include "mission_control.hpp"
#include "planet.hpp"

// Test unitaire

TEST(MissionControlUnitTest, ValidCommands) {
    MissionControl mc(0);
    EXPECT_TRUE(mc.IsValidCommand("F"));
    EXPECT_TRUE(mc.IsValidCommand("B"));
    EXPECT_TRUE(mc.IsValidCommand("L"));
    EXPECT_TRUE(mc.IsValidCommand("R"));
    EXPECT_TRUE(mc.IsValidCommand("FLBR"));
    EXPECT_FALSE(mc.IsValidCommand("X"));
    EXPECT_FALSE(mc.IsValidCommand("F1"));
    EXPECT_FALSE(mc.IsValidCommand("forward"));
}

TEST(MissionControlUnitTest, StringToObjectTypeConversion) {
    MissionControl mc(0);
    EXPECT_EQ(mc.StringToObjectType("ROVER"), ROVER);
    EXPECT_EQ(mc.StringToObjectType("OBSTACLE"), OBSTACLE);
    EXPECT_EQ(mc.StringToObjectType("EMPTY"), EMPTY);
    EXPECT_EQ(mc.StringToObjectType("UNKNOWN"), UNKNOWN);
    EXPECT_EQ(mc.StringToObjectType("invalid"), UNKNOWN);
}

TEST(MissionControlUnitTest, HasUnknownTilesDetection) {
    MissionControl mc(0);
    Planet p(5,5, true); 
    Tile** map = p.getMap();
    EXPECT_TRUE(mc.HasUnknownTiles(map, 5, 5));

   
    for(int x=0;x<5;x++){
        for(int y=0;y<5;y++){
            map[x][y].type = EMPTY;
        }
    }
    EXPECT_FALSE(mc.HasUnknownTiles(map, 5, 5));
}

TEST(MissionControlUnitTest, UpdateMapWithDiscoveredTiles) {
    MissionControl mc(0);
    Planet p(5,5);
    Tile** map = p.getMap();

    mc.UpdateMapWithDiscoveredTiles(map, 2, 3, OBSTACLE);
    EXPECT_EQ(map[2][3].type, OBSTACLE);

    mc.UpdateMapWithDiscoveredTiles(map, 0, 0, ROVER);
    EXPECT_EQ(map[0][0].type, ROVER);
}

// Test d'intégration

TEST(MissionControlIntegrationTest, DisplayMapRunsWithoutCrash) {
    MissionControl mc(0);
    Planet p(5,5);
    Tile** map = p.getMap();

    // Juste vérifier que ça ne crash pas
    EXPECT_NO_THROW(mc.DisplayMap(p.getWidth(), p.getHeight(), map, NORTH));
    EXPECT_NO_THROW(mc.DisplayMap(p.getWidth(), p.getHeight(), map, EAST));
    EXPECT_NO_THROW(mc.DisplayMap(p.getWidth(), p.getHeight(), map, SOUTH));
    EXPECT_NO_THROW(mc.DisplayMap(p.getWidth(), p.getHeight(), map, WEST));
}
