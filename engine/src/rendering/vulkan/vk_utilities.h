#pragma once

#include <vulkan/vulkan.h>
#include <fstream>

class  VulkanUtilities
{
private:
    /* data */
public:
    static bool LoadShaderModule(const std::string& filePath, VkDevice device, VkShaderModule& shaderModule);
};

bool VulkanUtilities::LoadShaderModule(const std::string& filePath, VkDevice device, VkShaderModule& outShaderModule)
{



    std::ifstream file(filePath, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        std::cout << "Failed to open file: " << filePath << "current:" << std::filesystem::current_path() << std::endl;
        return false;
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<uint32_t> buffer(fileSize/sizeof(uint32_t));

    file.seekg(0);
    file.read((char*)buffer.data(), fileSize);
    file.close();

    VkShaderModuleCreateInfo createInfo {VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO};
    createInfo.codeSize = fileSize;
    createInfo.pCode = buffer.data();

    VkShaderModule shaderModule;
    VkResult result = vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule);
    if (result!= VK_SUCCESS)
    {
        std::cout << "Failed to create shader module" << filePath  << " Result:" << result << std::endl;
        return false;
    }
    outShaderModule = shaderModule;
    return true;
}