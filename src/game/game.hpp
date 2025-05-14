#pragma once
#include "engine/app.hpp"
#include "engine/engine.hpp"

class Game: App{
    public:
    Game();
    void init();
    void update();
    void draw();
    private:
    Engine engine;
};
