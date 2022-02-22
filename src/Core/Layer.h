#pragma once

#include "../cglph.h"
#include "Event.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class Application;

class Layer
{
public:
	Layer(std::string name, Application* owner)
		: m_Name(name), m_Owner(owner) {}

	inline std::string GetName() { return m_Name; }
	inline Application* GetOwner() { return m_Owner; }

	// creation and destruction in application
	virtual void OnAttach() {};
	virtual void OnDetach() {};

	// application loop behaviour
	virtual void OnEvent(Event e) {};
	virtual void OnUpdate(float ts) {};
	virtual void OnRender() {};
	virtual void OnImGuiRender() {};

protected:
	Application* m_Owner;

private:
	std::string m_Name;
};