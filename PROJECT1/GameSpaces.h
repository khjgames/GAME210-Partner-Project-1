#pragma once
#include <chrono> // fancy thing that can get time with ms precision
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace GameTime {
    inline int dif(int a, int b) { return abs(a - b); }

    inline int tick() { // milliseconds since epoch, (based off lua function tick())
        auto now = std::chrono::system_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    }
}

namespace GameVars {
    //
    long long TimePlayed = 0;
    short NumPlayers = 1;
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
    bool GamePaused = false;
    bool ViewingLeaderboard = false;
    short GameOver = 0;
    short ScorePlaced = 0;
    string PlayerName = "NAME";
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
    void SaveGame(){
        // create an output stream
        ofstream output("SaveGameData.txt");
        if (output.fail()) {
            printf("Save failed");
        }
        else {
            output << to_string(VoidBits) << ",";
            output << to_string(TimePlayed) << ",";
            for (int num = 0; num < SHOP_UPGRADES; num++) {
                output << to_string(OwnedUpgrades[num]) << ",";
            }
        }
        output.close();
    }
    //
    void LoadGame(){
        ifstream input("SaveGameData.txt");
        if (input.fail()) SaveGame();
        else {
            string line; getline(input, line);

            short pos = 0; short found; int i = 0;

            if ((found = line.find(',')) != std::string::npos) {
                VoidBits = std::stoi(line.substr(0, found));
                pos = found + 1;
            }
            
            // Read the TimePlayed
            found = line.find(',', pos);
            if (found != std::string::npos) {
                TimePlayed = std::stoll(line.substr(pos, found - pos));
                pos = found + 1;
            }

            while (pos < line.size() && i < SHOP_UPGRADES) {
                if ((found = line.find(',', pos)) != std::string::npos) {
                    OwnedUpgrades[i] = std::stoi(line.substr(pos, found - pos));
                    pos = found + 1;
                }
                else {
                    OwnedUpgrades[i] = std::stoi(line.substr(pos));
                    break;
                }
                i++;
            }

            input.close();
        }
    }
    //
}