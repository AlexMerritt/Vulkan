#pragma once
#define NOMINMAX
#include "Log.h"

#include <vulkan\vulkan.h>
#include <algorithm>
#include <functional>
#include <GLFW\glfw3.h>
#include <vector>
#include <set>
#include <fstream>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_LUNARG_standard_validation"
};
const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");

	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");

	if (func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}

#ifdef _DEBUG
const bool validationEnabled = true;
#else
const bool validationEnabled = false;
#endif

template <typename T>
class VDeleter {
public:
	VDeleter() : VDeleter([](T _) {}) {}

	VDeleter(std::function<void(T, VkAllocationCallbacks*)> deletef) {
		this->deleter = [=](T obj) { deletef(obj, nullptr); };
	}

	VDeleter(const VDeleter<VkInstance>& instance, std::function<void(VkInstance, T, VkAllocationCallbacks*)> deletef) {
		this->deleter = [&instance, deletef](T obj) { deletef(instance, obj, nullptr); };
	}

	VDeleter(const VDeleter<VkDevice>& device, std::function<void(VkDevice, T, VkAllocationCallbacks*)> deletef) {
		this->deleter = [&device, deletef](T obj) { deletef(device, obj, nullptr); };
	}

	~VDeleter() {
		cleanup();
	}

	T* operator &() {
		cleanup();
		return &object;
	}

	operator T() const {
		return object;
	}

private:
	T object{ VK_NULL_HANDLE };
	std::function<void(T)> deleter;

	void cleanup() {
		if (object != VK_NULL_HANDLE) {
			deleter(object);
		}
		object = VK_NULL_HANDLE;
	}
};

struct QueueFamilyIndices
{
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool IsComplete()
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class Vulkan
{
public:
	bool Initialize(GLFWwindow* window, uint32_t width, uint32_t height);
	void Shutdown();

	void DrawFrame();

private:
	bool CreateInstance();
	bool CheckValidationLayerSupport();
	bool SetupDebugCallback();

	bool CreateSurface(GLFWwindow* window);

	bool SelectDevice();
	bool CreateDevice();

	bool CreateSwapChain(uint32_t width, uint32_t height);

	bool CreateImageViews();

	bool CreateRenderPass();

	bool CreateGraphicPipeline();
	bool CreateFrameBuffer();
	bool CreateCommandPool();
	bool CreateCommandBuffers();

	bool CreateSemaphores();

	std::vector<const char*> GetRequiredExtensions();
	bool IsDeviceValid(VkPhysicalDevice device);
	bool CheckDeviceExtensionsSupport(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height);

	void CreateShaderModule(const std::vector<char>& shaderSrc, VDeleter<VkShaderModule>& shaderModule);

	std::vector<char> ReadFile(const std::string filename);

private:
	VDeleter<VkInstance> m_instance{ vkDestroyInstance };
	VDeleter<VkDebugReportCallbackEXT> m_callback{ m_instance, DestroyDebugReportCallbackEXT };
	VDeleter<VkDevice> m_device{ vkDestroyDevice };
	VkPhysicalDevice m_physcalDevice = VK_NULL_HANDLE; // Since this will get disposed with the VkInstance does, we don't need to make this a VDeleter
	
	VDeleter<VkSurfaceKHR> m_surface{ m_instance, vkDestroySurfaceKHR };

	VDeleter<VkSwapchainKHR> m_swapChain{ m_device, vkDestroySwapchainKHR };
	std::vector<VkImage> m_swapChainImages;
	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;
	std::vector<VDeleter<VkImageView>> m_swapChainImageViews;

	VDeleter<VkPipelineLayout> m_pipelineLayout{ m_device, vkDestroyPipelineLayout };

	VDeleter<VkRenderPass> m_renderPass{ m_device, vkDestroyRenderPass };

	VDeleter<VkPipeline> m_graphicsPipeline{ m_device, vkDestroyPipeline };

	std::vector<VDeleter<VkFramebuffer>> m_swapChainFrameBuffers;

	VDeleter<VkCommandPool> m_commandPool{ m_device, vkDestroyCommandPool };
	std::vector<VkCommandBuffer> m_commandBuffers;

	VkQueue m_graphicsQueue; // Cleaned up when the logical devices is disposed
	VkQueue m_presentQueue;

	VDeleter<VkShaderModule> m_vertexShaderModule{ m_device, vkDestroyShaderModule };
	VDeleter<VkShaderModule> m_fragmentShaderModule{ m_device, vkDestroyShaderModule };

	VDeleter<VkSemaphore> m_imageAvailableSem{ m_device, vkDestroySemaphore };
	VDeleter<VkSemaphore> m_renderFinishedSem{ m_device, vkDestroySemaphore };

};

static VkBool32 debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) 
{
	Log::Error("Validation Layer: " + std::string(msg));
	return VK_FALSE;
}