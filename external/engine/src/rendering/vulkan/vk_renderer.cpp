#include "vk_renderer.h"
#include <VkBootstrap.h>
#include <unordered_map>
#include <engine/service_locator.h>
#include <iostream>
#include "vk_initializer.h"
#include "vk_utilities.h"
#include "vk_pipeline_builder.h"

void VulkanRenderer::Init(RendererSettings settings) {
    m_settings = settings;
    std::cout << "VulkanRenderer::Init()" << std::endl;
    initCore();
    std::cout << "VulkanRenderer::Init() - Done" << std::endl;
    initSwapchain();
    std::cout << "VulkanRenderer::Init() initSwapchian done" << std::endl;
    initRenderPass();
    std::cout << "VulkanRenderer::Init() initRenderPass done" << std::endl;
    initFramebuffers();
    std::cout << "VulkanRenderer::Init() initFramebuffers done" << std::endl;
    initCommandPool();
    std::cout << "VulkanRenderer::Init() initCommandPool done" << std::endl;
    initSyncObjects();
    std::cout << "VulkanRenderer::Init() initSyncObjects done" << std::endl;
    initPipeline();
    std::cout << "VulkanRenderer::Init() initPipeline done" << std::endl;
}

void VulkanRenderer::Shutdown() {
    vkDeviceWaitIdle(m_device);

    vkDestroyPipeline(m_device, m_triangle_pipeline, nullptr);
    vkDestroyPipelineLayout(m_device, m_triangle_pipeline_layout, nullptr);

    vkDestroyFence(m_device, m_render_fence, nullptr);
    vkDestroySemaphore(m_device, m_present_semaphore, nullptr);
    vkDestroySemaphore(m_device, m_render_semaphore, nullptr);

    for (auto framebuffer : m_framebuffers) {
        vkDestroyFramebuffer(m_device, framebuffer, nullptr);
    }

    vkDestroyRenderPass(m_device, m_render_pass, nullptr);
    vkDestroyCommandPool(m_device, m_command_pool, nullptr);
    vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);

    for(auto image_view : m_swapchain_image_views) {
        vkDestroyImageView(m_device, image_view, nullptr);
    }

    vkDestroyDevice(m_device, nullptr);
    vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    vkb::destroy_debug_utils_messenger(m_instance, m_debug_messenger, nullptr);
    vkDestroyInstance(m_instance, nullptr);
}

void VulkanRenderer::Render() {
    VK_CHECK(vkWaitForFences(m_device, 1, &m_render_fence, true, 1000000000)); // 1
    VK_CHECK(vkResetFences(m_device, 1, &m_render_fence));                     // 0

    uint32_t swapchainImageIndex;
    VK_CHECK(vkAcquireNextImageKHR(m_device, m_swapchain, 1000000000, m_present_semaphore, VK_NULL_HANDLE, &swapchainImageIndex));

    VK_CHECK(vkResetCommandBuffer(m_main_command_buffer, 0));

    VkCommandBuffer cmd = m_main_command_buffer;

    VkCommandBufferBeginInfo beginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    VK_CHECK(vkBeginCommandBuffer(cmd, &beginInfo));

    float flashColour = abs(sin((float)m_frame_number / 120.f));

    VkClearValue clearValue{
        .color = {0.f, flashColour, flashColour, 1.f}};

    VkRenderPassBeginInfo renderPassBeginInfo{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    renderPassBeginInfo.renderPass = m_render_pass;
    renderPassBeginInfo.renderArea = {
        .offset = {
            .x = 0,
            .y = 0},
        .extent = m_window_extent};

    renderPassBeginInfo.framebuffer = m_framebuffers[swapchainImageIndex];
    // connect clear values
    renderPassBeginInfo.clearValueCount = 1;
    renderPassBeginInfo.pClearValues = &clearValue;

    vkCmdBeginRenderPass(cmd, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    // DRAW CALLS
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_triangle_pipeline);
    vkCmdDraw(cmd, 3, 1, 0, 0);

    vkCmdEndRenderPass(cmd);
    VK_CHECK(vkEndCommandBuffer(cmd));

    VkSubmitInfo submit{VK_STRUCTURE_TYPE_SUBMIT_INFO};

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    submit.pWaitDstStageMask = &waitStage;

    submit.waitSemaphoreCount = 1;
    submit.pWaitSemaphores = &m_present_semaphore;

    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = &m_render_semaphore;

    submit.commandBufferCount = 1;
    submit.pCommandBuffers = &m_main_command_buffer;

    VK_CHECK(vkQueueSubmit(m_graphics_queue, 1, &submit, m_render_fence));

    VkPresentInfoKHR presentInfoKhr{VK_STRUCTURE_TYPE_PRESENT_INFO_KHR};
    presentInfoKhr.swapchainCount = 1;
    presentInfoKhr.pSwapchains = &m_swapchain;

    presentInfoKhr.waitSemaphoreCount = 1;
    presentInfoKhr.pWaitSemaphores = &m_render_semaphore;

    presentInfoKhr.pImageIndices = &swapchainImageIndex;

    VK_CHECK(vkQueuePresentKHR(m_graphics_queue, &presentInfoKhr));
    m_frame_number++;
}

// private methods
void VulkanRenderer::initCore() {
    std::cout << "VulkanRenderer::initCore()" << std::endl;
    vkb::InstanceBuilder builder;
    auto buildInstance = builder.set_app_name(m_settings.ApplicationName.c_str())
        .request_validation_layers(true)
        .require_api_version(VK_API_VERSION_1_1)
        .use_default_debug_messenger()
        .build();

    if (!buildInstance) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }

    std::cout << "VulkanRenderer::initCore() buildInstance done" << std::endl;

    auto vkb_inst = buildInstance.value();
    m_instance = vkb_inst.instance;
    m_debug_messenger = vkb_inst.debug_messenger;

    std::cout << "VulkanRenderer::initCore() buildInstance.value() done" << std::endl;

    std::unordered_map<SurfaceArgs, std::any> surfaceArgs {
        { SurfaceArgs::INSTANCE, m_instance },
        { SurfaceArgs::OUT_SURFACE, &m_surface }
    };

    ServiceLocator::GetWindow()->RequestDrawSurface(surfaceArgs);

    std::cout << "VulkanRenderer::initCore() RequestDrawSurface done" << std::endl;

    // select physical device
    vkb::PhysicalDeviceSelector selector(vkb_inst);

    std::cout << "VulkanRenderer::initCore() PhysicalDeviceSelector" << std::endl;

    std::cout << "VulkanRenderer::initCore() PhysicalDeviceSelector 2" << std::endl;

    vkb::PhysicalDevice vkbPhysicalDevice {
        selector
            .set_minimum_version(1, 1)
            .set_surface(m_surface)
            .select()
            .value()
    };

    std::cout << "VulkanRenderer::initCore() PhysicalDeviceSelector done" << std::endl;

    // create the vulkan device
    vkb::DeviceBuilder deviceBuilder(vkbPhysicalDevice);
    vkb::Device vkbDevice { deviceBuilder.build().value() };

    m_device = vkbDevice.device;
    m_physical_device = vkbPhysicalDevice.physical_device;

    m_graphics_queue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
    m_grpahics_queue_family_index = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

    std::cout << "VulkanRenderer::initCore() DeviceBuilder done" << std::endl;
}

void VulkanRenderer::initSwapchain() {
    auto [width, height] = ServiceLocator::GetWindow()->GetSize();
    m_window_extent.width = width;
    m_window_extent.height = height;

    vkb::SwapchainBuilder swapchainBuilder{ m_physical_device, m_device, m_surface };
    vkb::Swapchain vkbSwapchain = swapchainBuilder
        .use_default_format_selection()
        .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
        .set_desired_extent(width, height)
        .build()
        .value();

    m_swapchain = vkbSwapchain.swapchain;
    m_swapchain_images = vkbSwapchain.get_images().value();
    m_swapchain_image_views = vkbSwapchain.get_image_views().value();
    m_swapchain_format = vkbSwapchain.image_format;
}

void VulkanRenderer::initCommandPool() {
    VkCommandPoolCreateInfo createInfo {VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO};
    createInfo.queueFamilyIndex = m_grpahics_queue_family_index;
    createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    if (vkCreateCommandPool(m_device, &createInfo, nullptr, &m_command_pool) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create command pool");
    }

    VkCommandBufferAllocateInfo allocInfo {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    allocInfo.commandPool = m_command_pool;
    allocInfo.commandBufferCount = 1;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    if (vkAllocateCommandBuffers(m_device, &allocInfo, &m_main_command_buffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers");
    }
}

void VulkanRenderer::initRenderPass() {
    VkAttachmentDescription colorAttachment {};
    colorAttachment.format = m_swapchain_format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkRenderPassCreateInfo renderPassInfo {VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_render_pass) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create render pass");
    }
}

void VulkanRenderer::initFramebuffers() {
    VkFramebufferCreateInfo framebufferInfo {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    framebufferInfo.renderPass = m_render_pass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.width = m_window_extent.width;
    framebufferInfo.height = m_window_extent.height;
    framebufferInfo.layers = 1;

    const uint32_t imageCount = m_swapchain_images.size();
    m_framebuffers.resize(imageCount);

    for (uint32_t i = 0; i < imageCount; i++) {
        framebufferInfo.pAttachments = &m_swapchain_image_views[i];
        if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_framebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create framebuffer");
        }
    }
}

void VulkanRenderer::initSyncObjects() {
    VkFenceCreateInfo fenceInfo {VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if(vkCreateFence(m_device, &fenceInfo, nullptr, &m_render_fence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create fence");
    }

    VkSemaphoreCreateInfo semaphoreInfo {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_present_semaphore) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create semaphore");
    }
    if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_render_semaphore) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create semaphore");
    }

}

void VulkanRenderer::initPipeline() {
    VkShaderModule triangleFragShader;
    if (!VulkanUtilities::LoadShaderModule("shaders/triangle.frag.spv", m_device, triangleFragShader)) {
        std::cout << "Failed to load triangle fragment shader module\n";
    }
    else {
        std::cout << "Successfully loaded triangle fragment shader module\n";
    }

    VkShaderModule triangleVertShader;
    if (!VulkanUtilities::LoadShaderModule("shaders/triangle.vert.spv", m_device, triangleVertShader)) {
        std::cout << "Failed to load triangle vertex shader module\n";
    }
    else {
        std::cout << "Successfully loaded triangle vertex shader module\n";
    }

    auto pipelineLayoutInfo = PipelineLayoutCreateInfo();
    VK_CHECK(vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_triangle_pipeline_layout));

    /*
     * TEMPORARY PIPELINE BUILDING
     */

    VulkanPipelineBuilder pipelineBuilder;
    pipelineBuilder._shaderStages.push_back(
        PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, triangleVertShader));
    pipelineBuilder._shaderStages.push_back(
        PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, triangleFragShader));

    pipelineBuilder._vertexInputInfo = PipelineVertexInputStateCreateInfo();
    pipelineBuilder._inputAssembly = PipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

    // build the viewport
    pipelineBuilder._viewport = {
        .x = 0.f,
        .y = 0.f,
        .width = static_cast<float>(m_window_extent.width),
        .height = static_cast<float>(m_window_extent.height),
        .minDepth = 0.f,
        .maxDepth = 1.f};

    pipelineBuilder._scissor = {
        .offset = {0, 0},
        .extent = m_window_extent};

    pipelineBuilder._rasterizer = PipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL);
    pipelineBuilder._multisampling = PipelineMultisampleStateCreateInfo();
    pipelineBuilder._colorBlendAttachment = PipelineColorBlendAttachmentState();
    pipelineBuilder._pipelineLayout = m_triangle_pipeline_layout;

    m_triangle_pipeline = pipelineBuilder.BuildPipeline(m_device, m_render_pass);

    vkDestroyShaderModule(m_device, triangleFragShader, nullptr);
    vkDestroyShaderModule(m_device, triangleVertShader, nullptr);
}