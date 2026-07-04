#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.h>

#include "graphics/window.hpp"
#include "primatives/engine_config.hpp"

namespace Nova {

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    uint32_t computeFamily;

    bool graphicsFamilyHasValue = false;
    bool presentFamilyHasValue = false;
    bool computeFamilyHasValue = false;

    bool isComplete() {
        return graphicsFamilyHasValue && presentFamilyHasValue && computeFamilyHasValue;
    }
};

class Device {
public:

  	Device(Window& window, const EngineConfig& engineConfig);
  	~Device();

  	// Not copyable or movable
  	Device(const Device&) = delete;
  	Device& operator=(const Device&) = delete;
  	Device(Device&&) = delete;
  	Device& operator=(Device&&) = delete;

  	VkCommandPool getCommandPool() { return commandPool; }
  	VkDevice device() { return device_; }
  	VkSurfaceKHR surface() { return surface_; }
  	VkQueue graphicsQueue() { return graphicsQueue_; }
  	VkQueue presentQueue() { return presentQueue_; }

  	SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
  	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
  	QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
  	VkFormat findSupportedFormat(
      	const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

  	// Buffer Helper Functions
  	void createBuffer(
      	VkDeviceSize size,
      	VkBufferUsageFlags usage,
      	VkMemoryPropertyFlags properties,
      	VkBuffer &buffer,
      	VkDeviceMemory &bufferMemory);
  	VkCommandBuffer beginSingleTimeCommands();
  	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
  	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
  	void copyBufferToImage(
      	VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

  	void createImageWithInfo(
      	const VkImageCreateInfo &imageInfo,
      	VkMemoryPropertyFlags properties,
      	VkImage &image,
      	VkDeviceMemory &imageMemory);

  	VkPhysicalDeviceProperties properties;
  	VkPhysicalDevice getPhysicalDevice();
  	VkInstance getInstance();

private:
  	void createInstance(const EngineConfig& c);
  	void setupDebugMessenger();
  	void createSurface(Window& window);
  	void pickPhysicalDevice(const EngineConfig& c);
  	void createLogicalDevice();
  	void createCommandPool();

  	// helper functions
  	bool isDeviceSuitable(VkPhysicalDevice device);
  	std::vector<const char *> getRequiredExtensions();
  	bool checkValidationLayerSupport();
  	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  	void hasGflwRequiredInstanceExtensions();
  	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

  	VkInstance instance                     = VK_NULL_HANDLE;
  	VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
  	VkPhysicalDevice physicalDevice         = VK_NULL_HANDLE;
  	VkCommandPool commandPool               = VK_NULL_HANDLE;
  	VkDevice device_                        = VK_NULL_HANDLE;
  	VkSurfaceKHR surface_                   = VK_NULL_HANDLE;
  	VkQueue graphicsQueue_                  = VK_NULL_HANDLE;
  	VkQueue presentQueue_                   = VK_NULL_HANDLE;

    bool enableValidationLayers;

  	const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
  	const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

}