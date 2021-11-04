#pragma once
#include <memory>
#include "platform/window.h"

class ServiceLocator {
public:
    static inline const std::unique_ptr<Window>& GetWindow() { return _window; }

    static inline void SetWindow(Window* window) {
        if (_window != nullptr) { return; }
        _window = std::unique_ptr<Window>(window);
    }

private:
    static inline std::unique_ptr<Window> _window = nullptr;
};