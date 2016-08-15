#pragma once
#include "Renderer.h"

class System
{
public:
	bool Initialize();
	void Run();
	void Release();

	void Update();

private:
	GLFWwindow* m_window; // Is included from vulkan.h. If glfw3.h is included I get macro redefintions. Should put this in its own class
	Renderer* m_renderer;
};

