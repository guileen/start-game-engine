#pragma once

#ifdef USE_SDL

#include <engine/platform/window.h>
#include <SDL.h>

class SDLWindow : public Window
{
public:
    SDLWindow() = default;
    virtual void OpenWindow(WindowData data) override;
    virtual bool Update() override;

    virtual std::pair<int, int> GetWindowExtents() override;
    virtual void RequestDrawSurface(std::unordered_map<SurfaceArgs, std::any> args) override;

private:
    SDL_Window *_window = nullptr;
};

#endif