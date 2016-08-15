#include "System.h"

bool System::Initialize()
{
	bool result;
	unsigned int width = 800;
	unsigned int height = 600;

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	m_window = glfwCreateWindow(width, height, "Vulkan Window", nullptr, nullptr);

	m_renderer = new Renderer();
	result = m_renderer->Initialize(m_window, width, height);
	if (!result)
	{
		return false;
	}

	return true;
}

void System::Run()
{
	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();

		Update();
	}
}

void System::Release()
{
	if (m_renderer)
	{
		m_renderer->Shutdown();
		delete m_renderer;
	}

	if (m_window)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
}

void System::Update()
{
	m_renderer->Draw();
}
