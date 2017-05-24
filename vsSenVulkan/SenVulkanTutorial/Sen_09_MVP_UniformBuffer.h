#pragma once

#ifndef __Sen_09_MVP_UniformBuffer__
#define __Sen_09_MVP_UniformBuffer__

#include "../Support/SenAbstractGLFW.h"

class Sen_09_MVP_UniformBuffer : public SenAbstractGLFW
{
public:
	Sen_09_MVP_UniformBuffer();
	virtual ~Sen_09_MVP_UniformBuffer();

protected:
	void initVulkanApplication() { ; }
	void reCreateRenderTarget() { ; } // for resize window
	void finalizeWidget() { ; }
	void updateUniformBuffer() { ; }
	void cleanUpDepthStencil() { ; }

};

#endif // !__Sen_09_MVP_UniformBuffer__
