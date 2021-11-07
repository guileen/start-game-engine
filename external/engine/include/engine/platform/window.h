#pragma once

#include <unordered_map>
#include <any>

enum class SurfaceArgs {
    INSTANCE,
    ALLOCATORS,
    OUT_SURFACE
};

class Window {
public:
    virtual void OpenWindow() = 0;
    virtual void Update() = 0;
    virtual bool ShouldClose() = 0;

    virtual std::pair<int, int> GetSize() = 0;
    virtual void RequestDrawSurface(std::unordered_map<SurfaceArgs, std::any> args) = 0;
};