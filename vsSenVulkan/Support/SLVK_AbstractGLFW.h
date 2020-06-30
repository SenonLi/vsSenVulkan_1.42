#pragma once

#ifndef __SLVK_AbstractGLFW__
#define __SLVK_AbstractGLFW__

#if defined( _WIN32 )		// on Windows OS

#include <Windows.h>		// for OutputDebugString() function
//#define VK_USE_PLATFORM_WIN32_KHR 1 // For Vulkan Surface if not using GLFW

#elif defined( __linux )	// on Linux ( Via XCB library )
// xcb seems like a popular and well supported option on X11, until wayland and mir take over
#define VK_USE_PLATFORM_XCB_KHR 1
#include <xcb/xcb.h>
#endif

#include <stdexcept>// for propagating errors 
#include <stdlib.h>	// for const error catch
#include <stdio.h>  
#include <iostream> // for cout, cin
#include <fstream>	// for readFileBinaryStream function
#include <sstream>  // for quick chunch cout
#include <map>		// for ratePhysicalDevice to pick the best
#include <set>		// for unique queue family <==> unique queue
#include <vector>
#include <array>
#include <string>


#define GLFW_INCLUDE_VULKAN // Let GLFW know Vulkan is utilized, must be in front of vulkan.h
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>
#include <algorithm>		// std::max, std::min

//#define GLM_FORCE_RADIANS
// The perspective projection matrix generated by GLM will use the OpenGL depth range of -1.0 to 1.0 by default. 
// We need to configure it to use the Vulkan range of 0.0 to 1.0 using the GLM_FORCE_DEPTH_ZERO_TO_ONE definition.
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_SWIZZLE // Have to add this for new glm version without default structure initialization 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

#include <shaderc/shaderc.hpp>


class SLVK_AbstractGLFW
{
public:
	SLVK_AbstractGLFW();
	virtual ~SLVK_AbstractGLFW();

	void showWidget();

	static void createVulkanShaderModule(const VkDevice& logicalDevice, const std::string& diskFileAddress, VkShaderModule& shaderModule);
	static uint32_t findPhysicalDeviceMemoryPropertyIndex(
		const VkPhysicalDeviceMemoryProperties& gpuMemoryProperties,
		const VkMemoryRequirements& memoryRequirements,
		const VkMemoryPropertyFlags& requiredMemoryPropertyFlags
	);
	static void errorCheck(VkResult result, std::string msg);

	static void beginSingleTimeCommandBuffer(const VkCommandPool& tmpCommandBufferCommandPool, const VkDevice& logicalDevice,
		VkCommandBuffer& tempCommandBufferToBegin);
	static void endSingleTimeCommandBuffer(const VkCommandPool& tmpCommandBufferCommandPool, const VkDevice& logicalDevice,
		const VkQueue& tmpCommandBufferQueue, const VkCommandBuffer& tmpCommandBufferToEnd);

	static void createResourceBuffer(const VkDevice& logicalDevice, const VkDeviceSize& bufferDeviceSize,
		const VkBufferUsageFlags& bufferUsageFlags, const VkSharingMode& bufferSharingMode, const VkPhysicalDeviceMemoryProperties& gpuMemoryProperties,
		VkBuffer& bufferToCreate, VkDeviceMemory& bufferDeviceMemoryToAllocate, const VkMemoryPropertyFlags& requiredMemoryPropertyFlags);
	static void transferResourceBuffer(const VkCommandPool& bufferTransferCommandPool, const VkDevice& logicalDevice, const VkQueue& bufferMemoryTransferQueue,
		const VkBuffer& srcBuffer, const VkBuffer& dstBuffer, const VkDeviceSize& resourceBufferSize);
	static void transferResourceBufferToImage(const VkCommandPool& bufferToImageCommandPool, const VkQueue& bufferToImageTransferQueue
		,const VkDevice& logicalDevice, const VkBuffer& srcBuffer, const VkImage& dstImage, const uint32_t& imageWidth, const uint32_t& imageHeight
		, const uint32_t& layerCount , const uint32_t& regionCount , const VkBufferImageCopy* ptrBufferImageCopyRegionVec);

	static const std::vector<VkFormat> depthStencilSupportCheckFormatsVector;
	static bool hasStencilComponent(VkFormat formatToCheck);
	static void createResourceImage(const VkDevice& logicalDevice, const uint32_t& imageWidth, const uint32_t& imageHeight
		, const VkImageType& imageType, const VkFormat& imageFormat, const VkImageTiling& imageTiling, const VkImageUsageFlags& imageUsageFlags
		, VkImage& imageToCreate, VkDeviceMemory& imageDeviceMemoryToAllocate, const VkMemoryPropertyFlags& requiredMemoryPropertyFlags
		, const VkSharingMode& imageSharingMode, const VkPhysicalDeviceMemoryProperties& gpuMemoryProperties, const uint32_t& layerCount);
	static void transitionResourceImageLayout(const VkImage& imageToTransitionLayout, const VkImageSubresourceRange& imageSubresourceRangeToTransition
		, const VkImageLayout& oldImageLayout, const VkImageLayout& newImageLayout
		, const VkDevice& logicalDevice, const VkCommandPool& transitionImageLayoutCommandPool, const VkQueue& imageMemoryTransferQueue);
	static void transferResourceImage(const VkCommandPool& imageTransferCommandPool, const VkDevice& logicalDevice, const VkQueue& imageTransferQueue,
		const VkImage& srcImage, const VkImage& dstImage, const uint32_t& imageWidth, const uint32_t& imageHeight);
	static void createDeviceLocalTexture(const VkDevice& logicalDevice, const VkPhysicalDeviceMemoryProperties& gpuMemoryProperties
		, const char*& textureDiskAddress, const VkImageType& imageType, int& textureWidth, int& textureHeight
		, VkImage& deviceLocalTextureToCreate, VkDeviceMemory& textureDeviceMemoryToAllocate, VkImageView& textureImageViewToCreate
		, const VkSharingMode& imageSharingMode, const VkCommandPool& tmpCommandBufferCommandPool, const VkQueue& imageMemoryTransferQueue);
	static void createTextureSampler(const VkDevice& logicalDevice, VkSampler& textureSamplerToCreate);

	static void createDeviceLocalTextureArray(const VkDevice& logicalDevice, const VkPhysicalDeviceMemoryProperties& gpuMemoryProperties
		, const std::vector<std::string> & texturesDiskAddressVector, const VkImageType& imageType
		, VkImage& deviceLocalTextureToCreate, VkDeviceMemory& textureDeviceMemoryToAllocate, VkImageView& textureImageViewToCreate
		, const VkSharingMode& imageSharingMode, const VkCommandPool& tmpCommandBufferCommandPool, const VkQueue& imageMemoryTransferQueue);

protected:
	virtual void initVulkanApplication()	= 0;
	virtual void cleanUpDepthStencil()		= 0;
	virtual void reCreateRenderTarget()		= 0; // for resize window
	virtual void finalizeWidget()			= 0;
	virtual void updateUniformBuffer()		= 0;
	virtual void onKeyboardReaction(GLFWwindow* widget, int key, int scancode, int action, int mode);

	const int DEFAULT_widgetWidth	= 800;	// 640;
	const int DEFAULT_widgetHeight	= 600;	// 640;

	void createColorAttachOnlyRenderPass();
	void createColorAttachOnlySwapchainFramebuffers();
	void createDefaultCommandPool();
	void createSingleRectIndexBuffer();
	void createMvpUniformBuffers();
	/*****************************************************************************************************************/
	/*-----------             Depth Test FrameBuffer related            ---------------------------------------------*/
	/*---------------------------------------------------------------------------------------------------------------*/
	void createDepthTestAttachment();
	void createDepthTestRenderPass();
	void createDepthTestSwapchainFramebuffers();

	VkRenderPass					depthTestRenderPass					= VK_NULL_HANDLE;
	VkImage							depthTestImage						= VK_NULL_HANDLE;
	VkDeviceMemory					depthTestImageDeviceMemory			= VK_NULL_HANDLE;
	VkImageView						depthTestImageView					= VK_NULL_HANDLE;
	VkFormat						depthTestFormat						= VK_FORMAT_UNDEFINED;
	bool							hasStencil							= false;
	VkImageSubresourceRange			depthTestImageSubresourceRange{};
	/*****************************************************************************************************************/

#ifdef _DEBUG
	// Enable validation layers for debugging
	const bool DEBUG_LAYERS_ENABLED = true;
#else			
	const bool DEBUG_LAYERS_ENABLED = false;
#endif

	GLFWwindow* widgetGLFW;
	int m_WidgetWidth, m_WidgetHeight;
	const char* strWindowName;
	VkInstance						instance					= VK_NULL_HANDLE;
	/*******************************************************************************************************************************/
	/********* VkSurfaceKHR object that represents an abstract type of surface to present rendered images to. **********************/
	/********* The surface in our program will be backed by the window that we've already opened with GLFW.   **********************/
	VkSurfaceKHR					m_Surface						= VK_NULL_HANDLE; // VK_KHR_surface Instance Extension
	VkSurfaceFormatKHR				m_SurfaceFormat{};
	VkSurfaceCapabilitiesKHR		m_SurfaceCapabilities{};
	
	VkPhysicalDevice					m_PhysicalDevice			= VK_NULL_HANDLE;
	VkPhysicalDeviceMemoryProperties	m_PhysicalDeviceMemoryProperties{};
	int32_t								graphicsQueueFamilyIndex	= -1;	// Index of Graphics QueueFamily of GPU that we will choose to 
	int32_t								presentQueueFamilyIndex		= -1;	// The Graphics (Drawing) QueueFamily may not support presentation (WSI)

	/********** Default Logical Device ********************************************************/
	VkDevice						m_LogicalDevice				= VK_NULL_HANDLE;
	// GPU QueueFamilies can accept different types of work, which means we could assign One form of work in One Unique queue
	// (e.g.DMA / memoryTransfer-only queue);    Queues are on GPU, auto "multi-threads"
	VkQueue							m_GraphicsQueue				= VK_NULL_HANDLE;			// Handle to the graphics queue
	VkQueue							m_SwapchainPresentQueue		= VK_NULL_HANDLE;			// Since presentQueueFamilyIndex may not == graphicsQueueFamilyIndex, make two queue
	VkPresentModeKHR				m_SwapchainPresentMode		= VK_PRESENT_MODE_FIFO_KHR; // VK_PRESENT_MODE_FIFO_KHR is always available.

	VkSwapchainKHR					m_SwapChain					= VK_NULL_HANDLE;
	uint32_t						m_SwapChain_ImagesCount		= 2;
	VkViewport						m_SwapchainResize_Viewport{};
	VkRect2D						m_SwapchainResize_ScissorRect2D{};
	std::vector<VkImageView>		m_SwapchainImageViewsVector;	// m_SwapchainImageViewsVector has the same life length as m_SwapchainFramebufferVector
	std::vector<VkFramebuffer>		m_SwapchainFramebufferVector;
	std::vector<VkCommandBuffer>	m_SwapchainCommandBufferVector;
	/**** Three Default (no need to change) SwapChain (SC) Synchronization Primitives:  one VkFence vector, two VkSemaphore ***/
	std::vector<VkFence>			m_SC_WaitCommandBufferCompleteFencesVector;
	VkSemaphore m_SC_ImageAcquiredSemaphore;		// wait for SWI, from VK_IMAGE_LAYOUT_PRESENT_SRC_KHR to VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	VkSemaphore m_SC_PaintReadyToPresentSemaphore;	// wait for GPU, from VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL to VK_IMAGE_LAYOUT_PRESENT_SRC_KHR

	VkCommandPool					m_DefaultThreadCommandPool	= VK_NULL_HANDLE;
	VkRenderPass					m_ColorAttachOnlyRenderPass	= VK_NULL_HANDLE;
	VkBuffer						singleRectIndexBuffer		= VK_NULL_HANDLE;
	VkDeviceMemory					singleRectIndexBufferMemory = VK_NULL_HANDLE;

	// It should be noted that in a real world application, you're not supposed to actually call vkAllocateMemory for every individual buffer.
	// The maximum number of simultaneous memory allocations is limited by the maxMemoryAllocationCount physical m_LogicalDevice limit, 
	//		which may be as low as 4096 even on high end hardware like an NVIDIA GTX 1080.
	// The right way to allocate memory for a large number of objects at the same time is to 
	//		create a custom allocator that splits up a single allocation among many different objects by using the offset parameters
	//		that we've seen in many functions.

	// As mentioned above that you should allocate multiple resources like buffers from a single memory allocation, 
	//		in fact, you should go a step further.
	//	Driver developers recommend that you also store multiple buffers, like the vertex and index buffer,
	//		into a single VkBuffer and use offsets in commands like vkCmdBindVertexBuffers.
	//	The advantage is that your data is more cache friendly in that case, because it's closer together.
	//	It is even possible to reuse the same chunk of memory for multiple resources if they are not used during the same render operations,
	//		provided that their data is refreshed, of course.
	// This is known as aliasing and some Vulkan functions have explicit flags to specify that you want to do this.
	
	/*****************************************************************************************************************/
	/*-----------     Necessary Structures for Resources Descrition       -------------------------------------------*/
	/*---------------------------------------------------------------------------------------------------------------*/
	struct MvpUniformBufferObject {
		glm::mat4 model			= glm::mat4(1.0f);
		glm::mat4 view			= glm::mat4(1.0f);
		glm::mat4 projection	= glm::mat4(1.0f);
	};

	const int						m_UniformBuffer_DS_Index			= 0;
	VkBuffer						mvpUniformStagingBuffer				= VK_NULL_HANDLE;
	VkDeviceMemory					mvpUniformStagingBufferDeviceMemory	= VK_NULL_HANDLE;
	VkBuffer						mvpOptimalUniformBuffer				= VK_NULL_HANDLE;
	VkDeviceMemory					mvpOptimalUniformBufferMemory		= VK_NULL_HANDLE;

	/*****************************************************************************************************************/
	/*-----------             Depth Stencil FrameBuffer related, Not Applicable Yet    ------------------------------*/
	/*---------------------------------------------------------------------------------------------------------------*/
	VkImage								depthStencilImage				= VK_NULL_HANDLE;
	VkDeviceMemory						depthStencilImageDeviceMemory	= VK_NULL_HANDLE;
	VkImageView							depthStencilImageView			= VK_NULL_HANDLE;
	VkFormat							depthStencilFormat				= VK_FORMAT_UNDEFINED;
	bool								stencilAvailable				= false;

	void setImageMemoryBarrier(VkImage image, VkImageAspectFlags imageAspectFlags
		, VkImageLayout oldImageLayout, VkImageLayout newImageLayout
		, VkAccessFlagBits srcAccessFlagBits, const VkCommandBuffer& imageLayoutTransitionCommandBuffer);
	void createDepthStencilAttachment();
	void createDepthStencilRenderPass();
	void createDepthStencilGraphicsPipeline();
	/*****************************************************************************************************************/

private:
	static void onWidgetResized(GLFWwindow* widget, int width, int height);
	static void onKeyboardDetected(GLFWwindow* widget, int key, int scancode, int action, int mode);
	static VKAPI_ATTR VkBool32 VKAPI_CALL pfnDebugCallback(VkFlags, VkDebugReportObjectTypeEXT, uint64_t, size_t, int32_t, const char*, const char*, void *);

	static std::vector<char> readFileStream(const std::string& diskFileAddress, bool binary = false);
	static void createShaderModuleFromSPIRV(const VkDevice& logicalDevice, const std::vector<char>& SPIRV_Vector, VkShaderModule& shaderModule);
	static std::vector<uint32_t> shadercToSPIRV(const std::string& source_name, shaderc_shader_kind kind, const std::string& source, bool optimize = true);

	std::vector<const char*> debugInstanceLayersVector;
	std::vector<const char*> debugInstanceExtensionsVector;
	std::vector<const char*> debugDeviceLayersVector; 		// depricated, but still recommended
	std::vector<const char*> debugDeviceExtensionsVector;

	VkDebugReportCallbackCreateInfoEXT	debugReportCallbackCreateInfo{}; // important for creations of both instance and debugReportCallback
	VkDebugReportCallbackEXT			debugReportCallback						= VK_NULL_HANDLE;
	PFN_vkCreateDebugReportCallbackEXT	fetch_vkCreateDebugReportCallbackEXT	= VK_NULL_HANDLE;
	PFN_vkDestroyDebugReportCallbackEXT	fetch_vkDestroyDebugReportCallbackEXT	= VK_NULL_HANDLE;

	void initGlfwVulkanDebugWSI();
	void initDebugLayers();
	void initExtensions();
	void createInstance();
	void initDebugReportCallback();
	/*******************************************************************************************************************************/
	/********* The window m_Surface needs to be created right after the instance creation, *******************************************/
	/********* because it can actually influence the VkPhysicalDevice selection.          *******************************************/
	void createSurface(); // m_Surface == default framebuffer to draw

	void showPhysicalDeviceInfo(const VkPhysicalDevice& gpuToCheck);
	bool isPhysicalDeviceSuitable(const VkPhysicalDevice& gpuToCheck, int32_t& graphicsQueueIndex, int32_t& presentQueueIndex);
	int  ratePhysicalDevice(const VkPhysicalDevice& gpuToCheck, int32_t& graphicsQueueIndex, int32_t& presentQueueIndex);
	void pickPhysicalDevice();
	void createDefaultLogicalDevice();

	void collectSwapchainFeatures();
	void createSwapchain();
	void cleanUpSwapChain();
	void createSynchronizationPrimitives();
	void swapSwapchain();

	void reInitPresentation();
	void finalizeAbstractGLFW();
	/*******************************************************************************************************************************/
	bool checkInstanceLayersSupport(std::vector<const char*> layersVector);
	// Enumerate All
	void showAllSupportedInstanceExtensions();
	void showAllSupportedInstanceLayers();
	void showAllSupportedExtensionsEachUnderInstanceLayer();
	void showPhysicalDeviceSupportedLayersAndExtensions(const VkPhysicalDevice& gpuToCheck);
};


#endif //__SenAbstractGLFW__