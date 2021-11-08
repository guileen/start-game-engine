#pragma once
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <engine/platform/window.h>
#include <string>
#include <input/glfw_input.h>

class CustomWindow : public Window {
public:
    CustomWindow() = default;
    virtual void OpenWindow(WindowData data) override;
    virtual bool Update() override;
    virtual std::pair<int, int> GetWindowExtents() override;
    virtual void RequestDrawSurface(std::unordered_map<SurfaceArgs, std::any> args) override;

private:
    GLFWwindow* _window;

    std::unordered_map<InputKey, InputDeviceState> getGamepadState(int joystickId);
    MultiplatformInput _input {};
};