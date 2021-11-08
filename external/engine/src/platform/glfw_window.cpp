#include "glfw_window.h"
#include <iostream>
#include <engine/service_locator.h>

void CustomWindow::OpenWindow(WindowData data) {
    std::cout << "OpenWindow" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    _window = glfwCreateWindow(data.width, data.height, data.title.c_str(), nullptr, nullptr);

    glfwSetWindowUserPointer(_window, &_input);
    for (int i = 0; i<GLFW_JOYSTICK_LAST; i++) {
        if(glfwJoystickPresent(i)) {
            std::cout << "Joystick " << i << " is present" << std::endl;
            auto* InputManager = ServiceLocator::GetInputManager();
            if(InputManager) {
                InputManager->RegisterDevice(InputDevice{
                    .type = InputDeviceType::GAMEPAD,
                    .index = i,
                    .stateFunc = std::bind(&CustomWindow::getGamepadState, this, std::placeholders::_1)
                });
            }
        }
    }


    /*
    glfwSetJoystickCallback([] (int joystickId, int event) {
        auto* inputManager = ServiceLocator::GetInputManager();
        if (inputManager) {
            auto *input = dynamic_cast<Window*>(ServiceLocator::GetWindow());
            if (input) {
                if (event == GLFW_CONNECTED && glfwJoystickIsGamepad(joystickId)) {
                    inputManager->RegisterDevice(InputDevice{
                        .type = InputDeviceType::GAMEPAD,
                        .index = joystickId,
                        .stateFunc = std::bind(&CustomWindow::getGamepadState, input, std::placeholders::_1)
                    });
                    std::cout << "Joystick " << joystickId << " is connected" << std::endl;
                }
                else if (event == GLFW_DISCONNECTED) {
                    inputManager->RemoveDevice(InputDeviceType::GAMEPAD, joystickId);
                    std::cout << "Joystick " << joystickId << " is disconnected" << std::endl;
                }
            }
        }
    });
    */

    glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto* input = static_cast<MultiplatformInput*>(glfwGetWindowUserPointer(window));

        if (input) {
            float value = 0.f;
            switch (action) {
                case GLFW_PRESS:
                case GLFW_REPEAT:
                    value = 1.f;
                    break;
                case GLFW_RELEASE:
                default:
                    value = 0.f;
                    break;
            }
            input->UpdateKeyboardState(key, value);
        }
    });

    glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
        auto* input = static_cast<MultiplatformInput*>(glfwGetWindowUserPointer(window));

        if (input) {
            input->UpdateMouseState(button, action == GLFW_PRESS ? 1.f : 0.f);
        }
    });

    // Register input devices
    auto* inputManager = ServiceLocator::GetInputManager();

    if (inputManager) {
        inputManager->RegisterDevice(InputDevice{
            .type = InputDeviceType::kKeyboard,
            .stateFunc = std::bind(&MultiplatformInput::GetKeyboardState, &_input, std::placeholders::_1)
        });
        inputManager->RegisterDevice(InputDevice{
            .type = InputDeviceType::kMouse,
            .stateFunc = std::bind(&MultiplatformInput::GetMouseState, &_input, std::placeholders::_1)
        });
    }
};

bool CustomWindow::Update() {
    glfwPollEvents();
    return glfwWindowShouldClose(_window);
};

std::pair<int, int> CustomWindow::GetWindowExtents() {
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    return {width, height};
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

std::unordered_map<InputKey, InputDeviceState> CustomWindow::getGamepadState(int joystickId) {
    GLFWgamepadstate state;
    if (glfwGetGamepadState(joystickId, &state)) {
        return _input.GetGamepadState(state);
    }

    return std::unordered_map<InputKey, InputDeviceState>{};
}