#pragma once

#include "Layer.h"

class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	// gestione layer
	void PushLayer(Layer* layer);
	void RemoveLayer(Layer* layer);
	void AddOverlay(Layer* layer);
	void LogLayers();

	// main loop utilities
	void OnEvent();
	void OnUpdate(float ts);
	void OnRender();

private:
	std::vector<Layer*> m_LayerStack;
};