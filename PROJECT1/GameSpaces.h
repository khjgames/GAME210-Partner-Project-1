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
    short ShipSpeed = 3;
    short ProjectileSpeed = 5;
    //
    short SpriteInt = 0;
    short SpriteIntMax = 200; // Sprite animation interval
    int LastTick;
    int Score;              // Current score
    short InvaderSpeed;       // Invader pixels per tick towards edge
    short DistPerAdvance;     // Pixels down to move after reach edge
    short AccelPerAdvance;    // InvaderSpeed increase after reach edge
    short AccelLowSurvivors;  // InvaderSpeed bonus per low survivor
    short LowSurvivors;       // Below x survivors earns that bonus 
    bool AdvancingLeft;
    bool AtMenu = true;
    string PlayerName = "AAA";
    string NameEntry = "PlayerName: ";
    string NameEdit = "";
    string StartBtnString = "Start Game";
    
    string CoopBtnString = "Players: " + to_string(NumPlayers) + "P";
    string Typeset[] = { "0","1","2","3","4","5","6","7","8","9","0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z" };
    const short TYPESET_SIZE = 46;
    //
    void InitGameVars() {
        Score = 0;
        InvaderSpeed = 1;
        DistPerAdvance = 30;
        AccelPerAdvance = 0.5;
        AccelLowSurvivors = 1;
        LowSurvivors = 6;
        AdvancingLeft = true;
    };
}