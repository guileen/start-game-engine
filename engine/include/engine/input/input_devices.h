#pragma once
#include "input_key.h"
#include <functional>
#include <unordered_map>

enum class InputDeviceType : int32_t {
    kKeyboard,
    kMouse,
    GAMEPAD
};

struct InputDeviceState {
    float value {-99.f};
};

using InputDeviceStateCallbackFunc = std::function<std::unordered_map<InputKey, InputDeviceState>(int)>;

struct  InputDevice {
    InputDeviceType type;
    int index;
    std::unordered_map<InputKey, InputDeviceState> states;
    InputDeviceStateCallbackFunc stateFunc;
};
