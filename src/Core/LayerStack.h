#pragma once

#include "Layer.h"
#include "Event.h"

class Application;

class LayerStack
{
public:
	LayerStack(Application* owner);
	~LayerStack();

	// gestione layer
	void PushLayer(Layer* layer);
	void RemoveLayer(Layer* layer);
	void AddOverlay(Layer* layer);
	void LogLayers();

	// main loop utilities
	void OnEvent(const std::vector<Event>& EventQueue);
	void OnUpdate(float ts);
	void OnRender();

private:
	std::vector<Layer*> m_LayerStack;
	Application* m_Owner;
};