#pragma once

#include "../Core/Layer.h"
#include "../Renderer/Shader.h"

class TestLayer : public Layer
{
public:
	TestLayer(std::string name)
		: Layer(name), m_Shader(nullptr), m_VAO(0), m_VBO(0) {}

	// creazione e distruzione
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	// loop di funzionamento
	virtual void OnEvent(Event e) override;
	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;

private:
	uint32_t m_VAO, m_VBO;
	Shader* m_Shader;
};