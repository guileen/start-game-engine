#pragma once

class Window {
public:
    virtual void OpenWindow() = 0;
    virtual void Update() = 0;
    virtual bool ShouldClose() = 0;
};