#pragma once
#include "engine/app.hpp"
#include "engine/engine.hpp"
#include "flecs.h"

class Game: App{
    public:
    Game();
    void init();
    void update();
    void draw();
    private:
    Engine engine;
    flecs::world world;
    int jumpSpr;
};
