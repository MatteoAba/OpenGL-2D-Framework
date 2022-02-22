#pragma once

#include "Layer.h"
#include "Event.h"
#include "../Layers/ImGuiLayer.h"

class Application;

class LayerStack
{
public:
	LayerStack(Application* owner);
	~LayerStack();

	// laye management
	void PushLayer(Layer* layer);
	void RemoveLayer(Layer* layer);
	void AddOverlay(Layer* layer);
	void LogLayers();

	// main loop utilities
	void OnEvent(const std::vector<Event>& EventQueue);
	void OnUpdate(float ts);
	void OnRender();

private:
	ImGuiLayer* m_ImGuiLayer;
	std::vector<Layer*> m_LayerStack;
	Application* m_Owner;
};