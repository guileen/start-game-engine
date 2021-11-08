#pragma once

#include <memory>
#include <engine/platform/window.h>
#include <engine/rendering/renderer.h>
#include <engine/input/input_manager.h>

class ServiceLocator {
public:
    static inline Window* GetWindow() { return _window.get(); }
    static inline Renderer* GetRenderer() { return _renderer.get(); }
    static inline InputManager* GetInputManager() { return _inputManager.get(); }

    static inline void SetExePath(std::string path) { _exePath = path; }
    static inline std::string GetExePath() { return _exePath; }

    static inline void Provide(Window* window) {
        if (_window != nullptr) { return; }
        _window = std::unique_ptr<Window>(window);
    }

    static inline void Provide(Renderer* renderer, RendererSettings settings) {
        if (_renderer != nullptr) { return; }
        _renderer = std::unique_ptr<Renderer>(renderer);
        _renderer->Init(settings);
    }

    static inline void Provide(InputManager* inputManager) {
        if (_inputManager != nullptr) { return; }
        _inputManager = std::unique_ptr<InputManager>(inputManager);
    }

    static inline void Shutdown() {
        if(_inputManager != nullptr) { _inputManager.reset(); }

        _window.reset();
        _window = nullptr;

        _renderer->Shutdown();
        _renderer = nullptr;
    }

private:
    static inline std::unique_ptr<Window> _window = nullptr;
    static inline std::unique_ptr<Renderer> _renderer = nullptr;
    static inline std::unique_ptr<InputManager> _inputManager = nullptr;
    static inline std::string _exePath;
};