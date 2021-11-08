#pragma once
#include <string>

struct RendererSettings {
    std::string ApplicationName;
};

class Renderer {
public:
    virtual void Init(RendererSettings settings) = 0;
    virtual void Shutdown() = 0;
    virtual void Render() = 0;
};