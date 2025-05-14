#include "game.hpp"

Game::Game()
{
    engine.app = this;
    engine.init();
}

void Game::init() {}
void Game::update(){}
void Game::draw(){}
