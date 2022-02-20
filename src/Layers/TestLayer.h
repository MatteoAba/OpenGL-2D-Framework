#pragma once

#include "../Core/Layer.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Buffer.h"
#include "../Renderer/VertexArray.h"

class TestLayer : public Layer
{
public:
	TestLayer(std::string name, Application* owner)
		: Layer(name, owner), m_Show_demo_window(true) {}

	// creazione e distruzione
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	// loop di funzionamento
	virtual void OnEvent(Event e) override;
	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;

private:
	VertexBuffer* m_VBO;
	IndexBuffer* m_IBO;
	VertexArray* m_VAO;
	uint32_t m_FBO, m_TCB, m_RBO;
	Shader *m_Shader;
	bool m_Show_demo_window;
};