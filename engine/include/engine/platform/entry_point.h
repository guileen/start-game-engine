#pragma once
#include <iostream>
#include <engine/engine.h>
#include <engine/service_locator.h>
#include <glm/glm.hpp>

#include <engine/platform/game.h>

int main(int argc, char **argv) {
    // create the game
    auto* theGame = CreateGame();
    theGame->Run();
    delete theGame;
    return 0;
}