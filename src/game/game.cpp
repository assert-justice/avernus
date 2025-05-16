#include "game.hpp"

Game::Game()
{
    engine.app = this;
    engine.init();
}

void Game::init() {
    jumpSpr = engine.graphics.loadImage("sprites/Jump (32x32).png");
}
void Game::update(){}
void Game::draw(){
    engine.graphics.drawImage(jumpSpr, 0, 0);
}
