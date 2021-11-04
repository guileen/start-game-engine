#pragma once

#include <GLFW/glfw3.h>
#include <engine/platform/window.h>

class CustomWindow : public Window {
public:
    CustomWindow();
    virtual void OpenWindow() override;
    virtual bool Update() override;
private:
    GLFWwindow* _window;
};