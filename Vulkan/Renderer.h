#pragma once

#include "Vulkan.h"

class Renderer
{
public:
	bool Initialize(GLFWwindow* window, unsigned int width, unsigned int height);
	void Shutdown();

	void Draw();

private:
	Vulkan* m_vulkan;
};