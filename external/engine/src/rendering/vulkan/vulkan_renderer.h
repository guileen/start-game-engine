#pragma once

#include <engine/rendering/renderer.h>
// #include <vulkan/vulkan.h>
#include <vector>

class VulkanRenderer : public Renderer {
public:
    VulkanRenderer()=default;
    void Init(RendererSettings settings) override;
    void Shutdown() override;
    void Render() override;
};
