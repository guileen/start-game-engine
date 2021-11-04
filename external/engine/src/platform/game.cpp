#include "engine/platform/game.h"
#include "engine/service_locator.h"
#include "glfw_window.h"
#include <iostream>

Game::Game(): Game("New Engine Game") { }

Game::Game(std::string windowTitle): m_windowTitle(std::move(windowTitle)) {
    initializeServices();
}

void Game::Run() {
    // Open the window
    ServiceLocator::GetWindow()->OpenWindow();

    std::cout << "Game running..." << m_isRunning << std::endl;

    // run the application
    while (m_isRunning)
    {
        ServiceLocator::GetWindow()->Update();
        if(ServiceLocator::GetWindow()->ShouldClose())
        {
            m_isRunning = false;
        }
        Update(0.0f);
        // Update game state

        // Update physics
    }
    
}

void Game::initializeServices() {
    ServiceLocator::Provide(new CustomWindow(m_windowTitle, 800, 600));
}

void Game::shutdownServices() {
    ServiceLocator::Shutdown();
}