#include "LayerStack.h"
#include "Log.h"

LayerStack::LayerStack()
{
}

LayerStack::~LayerStack()
{
	for (Layer* layer : m_LayerStack) {
		layer->OnDetach();
		LOG_TRACE("Rimosso layer {}", layer->GetName());
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer)
{
	m_LayerStack.push_back(layer);
	layer->OnAttach();
	LOG_TRACE("Aggiunto layer {}", layer->GetName());
}

void LayerStack::RemoveLayer(Layer* layer)
{

}

void LayerStack::AddOverlay(Layer* layer)
{
	m_LayerStack.emplace(m_LayerStack.begin(), layer);
}

void LayerStack::LogLayers()
{
	LOG_WARN("ELENCO LAYER:");
	for (Layer* layer : m_LayerStack)
		LOG_WARN("[+] {}", layer->GetName());
}

void LayerStack::OnEvent()
{
	// eventi top - down
	for (int i = m_LayerStack.size() - 1; i >= 0; --i)
		m_LayerStack[i]->OnEvent();
}

void LayerStack::OnUpdate(float ts)
{
	// update top - down
	for (int i = m_LayerStack.size() - 1; i >= 0; --i)
		m_LayerStack[i]->OnUpdate(ts);
}

void LayerStack::OnRender()
{
	// render bottom - up
	for (int i = 0; i < m_LayerStack.size(); ++i)
		m_LayerStack[i]->OnRender();
}
