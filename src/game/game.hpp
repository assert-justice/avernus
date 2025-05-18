#pragma once
#include "engine/app.hpp"
#include "engine/engine.hpp"
#include "flecs.h"
#include "input.hpp"

class Game: App{
    public:
    Game();
    void init();
    void update();
    void draw();
    private:
    Engine engine;
    flecs::world ecs;
    std::vector<flecs::system> updateSystems;
    std::vector<flecs::system> drawSystems;
    int jumpSpr;
    int runSpr;
    Input input;
    float speed = 300;
    void initSystems();
};
