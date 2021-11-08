#pragma once
#include <iostream>
#include <filesystem>
#include <engine/engine.h>
#include <engine/service_locator.h>
#include <glm/glm.hpp>
#include <engine/log.h>
#include <engine/platform/game.h>

int main(int argc, char **argv) {
    auto dir = weakly_canonical(std::filesystem::path(argv[0])).parent_path();
    ServiceLocator::SetExePath(dir);
    info("Current working directory: %s", dir.c_str());
    // create the game
    auto* theGame = CreateGame();
    theGame->Run();
    delete theGame;
    return 0;
}