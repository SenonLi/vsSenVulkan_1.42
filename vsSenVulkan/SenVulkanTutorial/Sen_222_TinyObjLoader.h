#pragma once

#ifndef __Sen_222_TinyObjLoader__
#define __Sen_222_TinyObjLoader__

#include "../Support/SenAbstractGLFW.h"
#include "../Support/SenTinyObjLoader.h"

class Sen_222_TinyObjLoader :	public SenAbstractGLFW
{
public:
	Sen_222_TinyObjLoader();
	virtual ~Sen_222_TinyObjLoader();

protected:
	void initVulkanApplication();
	void reCreateRenderTarget(); // for resize window
	void finalizeWidget();

	void cleanUpDepthStencil();
	void updateUniformBuffer();

private:
	void createMeshLinkModelndexBuffer();
	void createMeshLinkModeVertexBuffer();
	void createTinyObjLoaderCommandBuffers();

	void initTinyObjCompleteTextureImage();
	void createTinyObjLoaderPipeline();
	void createTextureAppDescriptorPool();
	void createTextureAppDescriptorSetLayout();
	void createTextureAppDescriptorSet();

	VkImage							tinyObjCompleteImage				= VK_NULL_HANDLE;
	VkDeviceMemory					tinyObjCompleteImageDeviceMemory	= VK_NULL_HANDLE;
	VkImageView						tinyObjCompleteImageView			= VK_NULL_HANDLE;
	VkSampler						texture2DSampler					= VK_NULL_HANDLE;


	VkBuffer						tinyMeshLinkModelVertexBuffer		= VK_NULL_HANDLE;
	VkDeviceMemory					tinyMeshLinkModelVertexBufferMemory	= VK_NULL_HANDLE;
	VkBuffer						tinyMeshLinkModelIndexBuffer		= VK_NULL_HANDLE;
	VkDeviceMemory					tinyMeshLinkModelIndexBufferMemory	= VK_NULL_HANDLE;

	VkPipeline						tinyObjLoaderPipeline				= VK_NULL_HANDLE;

	VkPipelineLayout				tinyObjLoaderPipelineLayout			= VK_NULL_HANDLE;

	int tinyObjCompleteTextureWidth, tinyObjCompleteTextureHeight;
	const char* tinyObjCompleteTextureDiskAddress;
	const char* tinyObjectDiskAddress;

	std::vector<VertexStruct>	vertexStructVector;
	std::vector<uint32_t>		indexVector;
};


#endif // !__Sen_222_TinyObjLoader__

