#pragma once

#include "../cglph.h"
#include "Event.h"

class Application;

class Layer
{
public:
	Layer(std::string name, Application* owner)
		: m_Name(name), m_Owner(owner) {}

	inline std::string GetName() { return m_Name; }
	inline Application* GetOwner() { return m_Owner; }

	// creazione e distruzione
	virtual void OnAttach() {};
	virtual void OnDetach() {};

	// loop di funzionamento
	virtual void OnEvent(Event e) {};
	virtual void OnUpdate(float ts) {};
	virtual void OnRender() {};

private:
	std::string m_Name;
	Application* m_Owner;
};