#include <iostream>
#include <engine/engine.h>
#include <engine/service_locator.h>
#include <glm/glm.hpp>
// can't import cmake PRIVATE #include <GLFW/glfw3.h>

int main(int argc, char **argv) {
    std::cout << "Hello world" << std::endl;
    MyEngine::Init();
    ServiceLocator::GetWindow()->OpenWindow();
    while(!ServiceLocator::GetWindow()->Update()) {

    }
    return 0;
}