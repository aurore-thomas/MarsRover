#include <gtest/gtest.h>
#include "mission_control.hpp"
#include "planet.hpp"
#include "rover.hpp"

// Test unitaire

TEST(MissionControlUnitTest, ValidCommands) {
    MissionControl mc(8000);
    EXPECT_TRUE(mc.IsValidCommand("F"));
    EXPECT_TRUE(mc.IsValidCommand("B"));
    EXPECT_TRUE(mc.IsValidCommand("L"));
    EXPECT_TRUE(mc.IsValidCommand("R"));
    EXPECT_TRUE(mc.IsValidCommand("FLR"));
    EXPECT_FALSE(mc.IsValidCommand("X"));
    EXPECT_FALSE(mc.IsValidCommand("FRX"));
    EXPECT_FALSE(mc.IsValidCommand("forward"));
}

TEST(MissionControlUnitTest, StringToObjectTypeConversion) {
    MissionControl mc(8001);
    Planet p(5,5,true);
    EXPECT_EQ(mc.StringToObjectType("ROVER"), ROVER);
    EXPECT_EQ(mc.StringToObjectType("OBSTACLE"), OBSTACLE);
    EXPECT_EQ(mc.StringToObjectType("EMPTY"), EMPTY);
    EXPECT_EQ(mc.StringToObjectType("UNKNOWN"), UNKNOWN);
    EXPECT_EQ(mc.StringToObjectType("invalid"), UNKNOWN);
}

TEST(MissionControlUnitTest, HasUnknownTilesDetection) {
    MissionControl mc(8002);
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

// Test d'intégration

TEST(MissionControlIntegrationTest, DisplayMapRunsWithoutCrash) {
    MissionControl mc(8003);
    Planet p(5,5);

    EXPECT_NO_THROW(mc.DisplayMap(p.getWidth(), p.getHeight(), p.getMap(), NORTH));
    EXPECT_NO_THROW(mc.DisplayMap(p.getWidth(), p.getHeight(), p.getMap(), EAST));
    EXPECT_NO_THROW(mc.DisplayMap(p.getWidth(), p.getHeight(), p.getMap(), SOUTH));
    EXPECT_NO_THROW(mc.DisplayMap(p.getWidth(), p.getHeight(), p.getMap(), WEST));
}
