#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../cglph.h"
#include "Log.h"
#include "../Renderer/Window.h"
#include "Event.h"

// ------- TEMP ---------------
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
// ----------------------------


class Application
{
public:
	Application(int width, int height, const std::string& title);
	~Application();

	void Inizialize();
	void Run();

	inline void setRunning(bool state) { m_Running = state; }
	inline void pushEvent(Event e) { m_EventQueue.push_back(e); }

private:
	// main loop
	void OnEvent();
	void OnUpdate();
	void OnRender();

private:
	bool m_Running;
	Window* m_Window;
	float m_LastFrame, m_TimeStep;
	std::vector<Event> m_EventQueue;
};