#include "glfw_window.h"
#include <iostream>

CustomWindow::CustomWindow() {
    std::cout << "Custom Window" << std::endl;
    _window = nullptr;
}

void CustomWindow::OpenWindow() {
    std::cout << "OpenWindow" << std::endl;
    glfwInit();
    _window = glfwCreateWindow(800, 600, "My engine window", nullptr, nullptr);
};

bool CustomWindow::Update() {
    glfwPollEvents();
    
    return glfwWindowShouldClose(_window);
    return false;
};