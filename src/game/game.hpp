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
    flecs::world ecs;
    flecs::system move_sys;
    flecs::system draw_sys;
    int jumpSpr;
};
