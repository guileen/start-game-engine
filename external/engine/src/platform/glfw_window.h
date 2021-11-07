#pragma once
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <engine/platform/window.h>
#include <string>

class CustomWindow : public Window {
public:
    CustomWindow();
    CustomWindow(std::string title, int width, int height);
    virtual void OpenWindow() override;
    virtual void Update() override;
    virtual bool ShouldClose() override;
    virtual std::pair<int, int> GetSize() override;
    virtual void RequestDrawSurface(std::unordered_map<SurfaceArgs, std::any> args) override;

private:
    GLFWwindow* _window;
    std::string _title;
    int _width;
    int _height;
};