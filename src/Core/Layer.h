#pragma once

#include "../cglph.h"
#include "Event.h"

class Layer
{
public:
	Layer(std::string name)
		: m_Name(name) {}
	inline std::string GetName() { return m_Name; }

	// creazione e distruzione
	virtual void OnAttach() {};
	virtual void OnDetach() {};

	// loop di funzionamento
	virtual void OnEvent(Event e) {};
	virtual void OnUpdate(float ts) {};
	virtual void OnRender() {};

private:
	std::string m_Name;
};