#include "game.hpp"

struct Position {
    double x;
    double y;
};

struct Velocity {
    double x;
    double y;
};

Game::Game()
{
    engine.app = this;
    engine.init();
}

void Game::init() {
    jumpSpr = engine.graphics.loadImage("sprites/Jump (32x32).png");
    move_sys = ecs.system<Position, Velocity>()
        .each([](flecs::iter& it, size_t, Position& p, Velocity& v) {
            p.x += v.x * it.delta_time();
            p.y += v.y * it.delta_time();
        });
    draw_sys = ecs.system<Position>()
        .each([this](flecs::iter& it, size_t, Position& p){
            engine.graphics.drawImage(jumpSpr, p.x, p.y);
        });
    auto bob = ecs.entity("Bob")
        .set(Position{0, 0})
        .set(Velocity{0,1});
}
void Game::update(){
    double dt = engine.getDeltaTime();
    move_sys.run(dt);
}
void Game::draw(){
    // engine.graphics.drawImage(jumpSpr, 0, 0);
    double ft = engine.getFrameTime();
    draw_sys.run(ft);
}
