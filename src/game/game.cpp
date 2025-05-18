#include "game.hpp"

struct Position {
    double x;
    double y;
};

struct Velocity {
    double x;
    double y;
};

struct Sprite{
    int id;
    int width;
    int height;
    int frames;
    double frameDuration;
    bool loops;
};

struct SprAnimated{
    int frame;
    bool hFlipped;
    double frameTime;
};

Game::Game()
{
    engine.app = this;
    engine.init();
}

void Game::init() {
    input.setWindow(engine.graphics.getWindow());
    jumpSpr = engine.graphics.loadImage("sprites/Jump (32x32).png");
    runSpr = engine.graphics.loadImage("sprites/Run (32x32).png");
    initSystems();
    auto bob = ecs.entity("Bob")
        .set(Position{0, 0})
        .set(Velocity{0,0})
        .set(Sprite{runSpr, 32, 32, 12, 1.0/20, true})
        .set(SprAnimated{0, true, 0});
}
void Game::update(){
    input.poll();
    if(input.isMenuPressed()) engine.quit();
    double dt = engine.getDeltaTime();
    for(auto sys : updateSystems){
        sys.run(dt);
    }
}
void Game::draw(){
    double ft = engine.getFrameTime();
    for(auto sys : drawSystems){
        sys.run(ft);
    }
}

void Game::initSystems()
{
    auto playerSys = ecs.system<Velocity, SprAnimated>()
        .each([this](flecs::iter& it, size_t, Velocity& v, SprAnimated& a){
            auto move = input.move();
            move *= speed;
            if(move.x > 0) a.hFlipped = false;
            if(move.x < 0) a.hFlipped = true;
            v.x = move.x;
            v.y = move.y;
        });
    auto moveSys = ecs.system<Position, Velocity>()
        .each([](flecs::iter& it, size_t, Position& p, Velocity& v) {
            p.x += v.x * it.delta_time();
            p.y += v.y * it.delta_time();
        });
    updateSystems.push_back(playerSys);
    updateSystems.push_back(moveSys);
    auto drawSys = ecs.system<Position, Sprite, SprAnimated>()
        .each([this](flecs::iter& it, size_t, Position& p, Sprite& s, SprAnimated& a){
            a.frameTime += it.delta_time();
            if(a.frameTime > s.frameDuration){
                a.frameTime = 0;
                a.frame += 1;
                if(a.frame >= s.frames){
                    if(s.loops) a.frame = 0;
                    else a.frame = s.frames-1;
                }
            }
            float sx = s.width * a.frame;
            float sw = s.width;
            if(a.hFlipped){
                sx += sw;
                sw = -sw;
            }
            engine.graphics.drawImageExt(s.id, p.x, p.y, s.width*10, s.height*10, sx, 0, sw, s.height, 0, 0, 0);
        });
    drawSystems.push_back(drawSys);
}
