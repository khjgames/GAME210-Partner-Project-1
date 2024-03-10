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
    int ShipSpeed = 3;
    int ProjectileSpeed = 5;
    //
    int SpriteInt = 0;
    int SpriteIntMax = 200; // Sprite animation interval
    int LastTick;
    int Score;              // Current score
    int InvaderSpeed;       // Invader pixels per tick towards edge
    int DistPerAdvance;     // Pixels down to move after reach edge
    int AccelPerAdvance;    // InvaderSpeed increase after reach edge
    int AccelLowSurvivors;  // InvaderSpeed bonus per low survivor
    int LowSurvivors;       // Below x survivors earns that bonus 
    bool AdvancingLeft;
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