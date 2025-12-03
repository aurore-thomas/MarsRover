#include <gtest/gtest.h>
#include "mission_control.hpp"
#include "planet.hpp"
#include "rover.hpp"
/*
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
*/

#include <gtest/gtest.h>
#include "mission_control.hpp"
#include "planet.hpp"

// =========================================================
// 1. Définition des FIXTURES (Wrappers)
// =========================================================

class MissionControlUnitTest : public ::testing::Test {
protected:
    // Instance de MissionControl accessible par les tests
    // (Le constructeur ne doit pas être bloquant pour que ça marche)
    MissionControl mc{8000}; 

    // --- WRAPPERS (PASSE-PLATS) ---
    // Ces fonctions sont protected, donc accessibles par TEST_F
    // Elles appellent les fonctions privées de mc (car la classe est friend)

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
    
    // Pour DisplayMap, on veut juste vérifier que ça ne crash pas
    void CallDisplayMap(int width, int height, Tile** map, Orientation orientation) const {
        mc.DisplayMap(width, height, map, orientation);
    }
};

class MissionControlIntegrationTest : public ::testing::Test {
protected:
    MissionControl mc{8000};

    // On aura besoin de mettre à jour la map pour l'intégration
    void CallUpdateMap(Tile** map, int x, int y, ObjectType type) {
        mc.UpdateMapWithDiscoveredTiles(map, x, y, type);
    }
};


// =========================================================
// 2. Tests Unitaires (MissionControlUnitTest)
// =========================================================

TEST_F(MissionControlUnitTest, IsValidCommandTest) {
    // Utilisation du wrapper Call...
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
    // Test du cas par défaut/invalide
    EXPECT_EQ(CallStringToObjectType("Alien"), UNKNOWN);
}

TEST_F(MissionControlUnitTest, UpdateMapWithDiscoveredTilesTest) {
    Planet p(5, 5, true); // true = map vide (UNKNOWN)
    Tile** map = p.getMap();

    // Vérification avant
    EXPECT_EQ(map[2][2].type, UNKNOWN);

    // Action via wrapper
    CallUpdateMap(map, 2, 2, OBSTACLE);

    // Vérification après
    EXPECT_EQ(map[2][2].type, OBSTACLE);
}


// =========================================================
// 3. Tests de Classe (MissionControlClassTest)
// =========================================================

TEST_F(MissionControlClassTest, HasUnknownTilesTrueInitially) {
    Planet p(5, 5, true); // Map vide
    EXPECT_TRUE(CallHasUnknownTiles(p.getMap(), 5, 5));
}

TEST_F(MissionControlClassTest, HasUnknownTilesFalseWhenFull) {
    Planet p(2, 2, true);
    Tile** map = p.getMap();

    // On remplit tout manuellement pour simuler une map finie
    map[0][0].type = EMPTY;
    map[0][1].type = EMPTY;
    map[1][0].type = OBSTACLE;
    map[1][1].type = ROVER;

    EXPECT_FALSE(CallHasUnknownTiles(map, 2, 2));
}

TEST_F(MissionControlClassTest, DisplayMapRunsWithoutCrash) {
    Planet p(3, 3);
    // On appelle juste pour vérifier qu'aucune exception n'est levée (segfault, etc.)
    EXPECT_NO_THROW(CallDisplayMap(3, 3, p.getMap(), NORTH));
}


// =========================================================
// 4. Tests Intégration (MissionControlIntegrationTest)
// =========================================================

TEST_F(MissionControlIntegrationTest, UpdateMapLogic) {
    Planet p(5, 5, true);
    Tile** map = p.getMap();

    // Simulation : MC reçoit une info et met à jour sa carte interne
    CallUpdateMap(map, 1, 1, ROVER);
    
    EXPECT_EQ(map[1][1].type, ROVER);
}