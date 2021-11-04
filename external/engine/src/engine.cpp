#include <engine/engine.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <engine/service_locator.h>
#include "platform/glfw_window.h"

void MyEngine::Init() {
    std::cout << "Initializing window" << std::endl;
    ServiceLocator::SetWindow(new CustomWindow());
};