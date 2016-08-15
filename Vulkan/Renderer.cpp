#include "Renderer.h"
#include "Log.h"

bool Renderer::Initialize(GLFWwindow* window, unsigned int width, unsigned int height)
{
	bool result;

	m_vulkan = new Vulkan();

	result = m_vulkan->Initialize(window, width, height);
	if (!result)
	{
		Log::Error("Unable to initialize vulkan");
		return false;
	}

	return true;
}

void Renderer::Shutdown()
{
	if (m_vulkan)
	{
		m_vulkan->Shutdown();
		delete m_vulkan;
	}
}

void Renderer::Draw()
{
	m_vulkan->DrawFrame();
}
