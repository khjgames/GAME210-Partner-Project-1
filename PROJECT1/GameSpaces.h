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
    int ShipSpeed = 3;
    int ProjectileSpeed = 5;
    //
    int Score;
    int InvaderSpeed;
    int DistPerAdvance;
    int AccelPerAdvance;
    int AccelLowSurvivors;
    int FrameSpeedBonus;
    int LastTick;
    bool AdvancingLeft = true;
}