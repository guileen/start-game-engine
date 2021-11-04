#include "glfw_window.h"
#include <iostream>

CustomWindow::CustomWindow() :CustomWindow("My Window", 800, 600) {}

CustomWindow::CustomWindow(std::string title, int width, int height) {
    std::cout << "Custom Window" << std::endl;
    _window = nullptr;
    _title = title;
    _width = width;
    _height = height;
}

void CustomWindow::OpenWindow() {
    std::cout << "OpenWindow" << std::endl;
    glfwInit();
    _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
};

void CustomWindow::Update() {
    glfwPollEvents();
};

bool CustomWindow::ShouldClose() {
    return glfwWindowShouldClose(_window);
};