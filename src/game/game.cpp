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
    input.setWindow(engine.graphics.getWindow());
    jumpSpr = engine.graphics.loadImage("sprites/Jump (32x32).png");
    move_sys = ecs.system<Position, Velocity>()
        .each([this](flecs::iter& it, size_t, Position& p, Velocity& v) {
            auto move = input.move();
            move *= speed;
            v.x = move.x;
            v.y = move.y;
            p.x += v.x * it.delta_time();
            p.y += v.y * it.delta_time();
        });
    draw_sys = ecs.system<Position>()
        .each([this](flecs::iter& it, size_t, Position& p){
            engine.graphics.drawImage(jumpSpr, p.x, p.y);
        });
    auto bob = ecs.entity("Bob")
        .set(Position{0, 0})
        .set(Velocity{0,0});
}
void Game::update(){
    input.poll();
    if(input.isMenuPressed()) engine.quit();
    double dt = engine.getDeltaTime();
    move_sys.run(dt);
}
void Game::draw(){
    // engine.graphics.drawImage(jumpSpr, 0, 0);
    double ft = engine.getFrameTime();
    draw_sys.run(ft);
}
