#pragma once

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
private:
    GLFWwindow* _window;
    std::string _title;
    int _width;
    int _height;
};