#pragma once
#include <chrono> // fancy thing that can get time with ms precision
#include <iostream>

namespace GameTime {
    inline int dif(int a, int b) { return abs(a - b); }

    inline int tick() { // milliseconds since epoch, (based off lua function tick())
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    }
}

namespace GameVars {
    //
    short NumPlayers = 2;
    short VoidBits = 0;
    short SpriteInt = 0;
    short SpriteIntMax = 200; // Sprite animation interval
    int LastTick;
    short Level;              // Current stage / level
    int Score;                // Current score
    short SpawnedUFOs;
    short ExtraSpawnedUFOs;
    short InvaderSpeed;       // Invader pixels per tick towards edge
    short DistPerAdvance;     // Pixels down to move after reach edge
    short AccelPerAdvance;    // InvaderSpeed increase after reach edge
    short AccelLowSurvivors;  // InvaderSpeed bonus per low survivor
    short LowSurvivors;       // Below x survivors earns that bonus 
    bool AdvancingLeft;
    bool AtMenu = true;
    bool AtShop = false;
    short GameOver = 0;
    bool ViewingLeaderboard = false;
    short ScorePlaced = 0;
    string PlayerName = "AAA";
    string NameEntry = "PlayerName: ";
    string NameEdit = "";
    string StartBtnString = "Start Game";
    string ShopBtnString = "Shop";
    string ExitShopBtnString = "Exit Shop";
    string CoopBtnString = "Players: " + to_string(NumPlayers) + "P";
    string Typeset[] = { "0","1","2","3","4","5","6","7","8","9","0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z" };
    const short TYPESET_SIZE = 46;
    //
    const short SHOP_UPGRADES = 4;
    short MaxUpgrades[SHOP_UPGRADES] = { 6, 7, 6, 4 };
    short OwnedUpgrades[SHOP_UPGRADES] = { 0, 0, 0, 0 };
    short UpgradeCosts[7] = { 15, 35, 75, 125, 185, 250, 250 };
    string UpgradeNames[SHOP_UPGRADES] = {"Ship Speed","Faster Shots","Missile Size","Study UFO"};
    string ShopBuyStrings[SHOP_UPGRADES] = {"Buy","Buy","Buy","Buy"};
    string ShopRefundStrings[SHOP_UPGRADES] = {"Refund","Refund", "Refund","Refund"};
    //
    short ShipSpeed = 3 + OwnedUpgrades[0];
    short ProjectileSpeed = 5;
    short ProjSizeX = 5;
    short ProjSizeY = 7;
    //
    void ApplyUpgrades() {
        ShipSpeed = 3 + floor(OwnedUpgrades[0] * 1.25);
        ProjectileSpeed = 5 + floor(OwnedUpgrades[1] * 1.7);
        ProjSizeX = floor(5 * pow(1.2, OwnedUpgrades[2]));
        ProjSizeY = floor(ProjSizeX * 7 / 5);
    };
    //
    void InitGameVars() {
        GameOver = 0;
        Level = 1;
        Score = 0;
        SpawnedUFOs = 0;
        ExtraSpawnedUFOs = 0;
        InvaderSpeed = 1;
        DistPerAdvance = 30;
        AccelPerAdvance = 0.5;
        AccelLowSurvivors = 1;
        LowSurvivors = 6;
        AdvancingLeft = true;
        ApplyUpgrades();
    };
    //
}