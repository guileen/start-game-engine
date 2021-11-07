#pragma once

#include <engine/rendering/renderer.h>
#include <vulkan/vulkan.h>
#include <vector>

#define VK_CHECK(x)                                                             \
    do {                                                                        \
        VkResult err = x;                                                       \
        if (err) {                                                              \
            std::cout << "Detected Vulkan error: " << err << std::endl;         \
            abort();                                                            \
        }                                                                       \
    } while(0)


class VulkanRenderer : public Renderer {
public:
    VulkanRenderer()=default;
    void Init(RendererSettings settings) override;
    void Shutdown() override;
    void Render() override;

private:
    void initCore();
    void initSwapchain();
    void initRenderPass();
    void initFramebuffers();
    void initCommandPool();
    void initSyncObjects();
    void initPipeline();

private:
    // temp frame number
    uint32_t m_frame_number = 0;
    RendererSettings m_settings {};

    // vulkan stuff
    VkInstance m_instance;
    VkDebugUtilsMessengerEXT m_debug_messenger;
    VkSurfaceKHR m_surface;
    VkPhysicalDevice m_physical_device;
    VkDevice m_device;

    // swapchain
    VkSwapchainKHR m_swapchain;
    VkFormat m_swapchain_format;
    VkExtent2D m_window_extent;
    std::vector<VkImage> m_swapchain_images;
    std::vector<VkImageView> m_swapchain_image_views;

    // command pool
    VkCommandPool m_command_pool;
    VkCommandBuffer m_main_command_buffer;
    VkQueue m_graphics_queue;
    uint32_t m_grpahics_queue_family_index;

    // render pass
    VkRenderPass m_render_pass;
    std::vector<VkFramebuffer> m_framebuffers {3};

    // sync objects
    VkSemaphore m_present_semaphore;
    VkSemaphore m_render_semaphore;
    VkFence m_render_fence;

    // pipeline
    VkPipeline m_triangle_pipeline;
    VkPipelineLayout m_triangle_pipeline_layout;
};
