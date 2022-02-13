#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../cglph.h"
#include "Log.h"
#include "../Renderer/Window.h"
#include "LayerStack.h"


class Application
{
public:
	Application(int width, int height, const std::string& title);
	~Application();

	void Inizialize();
	void Run();

	inline void SetRunning(bool state) { m_Running = state; }
	inline void PushEvent(Event e) { m_EventQueue.push_back(e); }

private:
	// main loop
	void OnEvent();
	void OnUpdate();
	void OnRender();

private:
	bool m_Running;
	Window* m_Window;
	LayerStack* m_LayerStack;
	float m_LastFrame, m_TimeStep;
	std::vector<Event> m_EventQueue;
};