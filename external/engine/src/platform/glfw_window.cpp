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
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);
};

void CustomWindow::Update() {
    glfwPollEvents();
};

bool CustomWindow::ShouldClose() {
    return glfwWindowShouldClose(_window);
};

std::pair<int, int> CustomWindow::GetSize() {
    return std::make_pair(_width, _height);
}

void CustomWindow::RequestDrawSurface(std::unordered_map<SurfaceArgs, std::any> args)
{
    try
    {
        auto vkInstance = std::any_cast<VkInstance>(args.at(SurfaceArgs::INSTANCE));
        auto *allocationCallbacks = args[SurfaceArgs::ALLOCATORS].has_value() ?
            std::any_cast<VkAllocationCallbacks*>(args.at(SurfaceArgs::ALLOCATORS)) :
            nullptr;
        auto *outSurface = std::any_cast<VkSurfaceKHR *> (args.at(SurfaceArgs::OUT_SURFACE));
        if (vkInstance == nullptr || outSurface == nullptr)
            throw std::runtime_error("Invalid arguments");

        if (glfwCreateWindowSurface(vkInstance, _window, allocationCallbacks, outSurface) != VK_SUCCESS)
            throw std::runtime_error("Failed to create window surface");
    }
    catch (std::bad_any_cast &e)
    {
        std::cout << "Failed to cast window surface arguments" << e.what() << std::endl;
    }
}
