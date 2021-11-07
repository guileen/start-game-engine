#include <engine/platform/game.h>
#include <engine/service_locator.h>
#include "platform/glfw_window.h"
#include "rendering/vulkan/vk_renderer.h"
#include "input/glfw_input.h"

#include <iostream>

Game::Game(): Game("New Engine Game") { }

Game::Game(std::string windowTitle): m_windowTitle(std::move(windowTitle)) {
    initializeServices();
}

void Game::Run() {
    std::cout << "Game running..." << m_isRunning << std::endl;

    // run the application
    while (m_isRunning)
    {
        ServiceLocator::GetWindow()->Update();
        if(ServiceLocator::GetWindow()->ShouldClose())
        {
            m_isRunning = false;
            continue;
        }
        
        if (ServiceLocator::GetInputManager()) {
            ServiceLocator::GetInputManager()->processInput();
        }
        // Update game state
        Update(0.0f);

        // Update physics

        // Render
        ServiceLocator::GetRenderer()->Render();
    }
    
}

void Game::initializeServices() {
    // provide input manager
    ServiceLocator::Provide(new InputManager());
    // provide window
    ServiceLocator::Provide(new CustomWindow(m_windowTitle, 800, 600));
    ServiceLocator::GetWindow()->OpenWindow();
    // provide renderer
    RendererSettings settings {
        .ApplicationName = m_windowTitle,
    };
    ServiceLocator::Provide(new VulkanRenderer(), settings);
}

void Game::shutdownServices() {
    ServiceLocator::Shutdown();
}