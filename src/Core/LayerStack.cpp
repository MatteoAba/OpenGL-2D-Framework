#include "LayerStack.h"
#include "Log.h"

LayerStack::LayerStack(Application* owner)
	: m_Owner(owner)
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

void LayerStack::OnEvent(const std::vector<Event>& EventQueue)
{
	// trasmetto tutti gli eventi nel buffer ai layer
	for (Event e : EventQueue) {
		// eventi top - down
		if (m_LayerStack.size())
			for (int i = (int)(m_LayerStack.size() - 1); i >= 0; --i)
				m_LayerStack[i]->OnEvent(e);
	}
}

void LayerStack::OnUpdate(float ts)
{
	// update top - down
	if (m_LayerStack.size())
		for (int i = (int)(m_LayerStack.size() - 1); i >= 0; --i)
			m_LayerStack[i]->OnUpdate(ts);
}

void LayerStack::OnRender()
{
	// render bottom - up
	if (m_LayerStack.size())
		for (int i = 0; i < (int)(m_LayerStack.size()); ++i)
			m_LayerStack[i]->OnRender();
}
