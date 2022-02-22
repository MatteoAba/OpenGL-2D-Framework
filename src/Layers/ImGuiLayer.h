#pragma once

#include "../cglph.h"
#include "../Core/Layer.h"

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer(std::string name, Application* owner);
	~ImGuiLayer();

	void Begin();
	void End();

private:
	std::string m_OpenGLVersion;
	bool m_ShowDemo;
};