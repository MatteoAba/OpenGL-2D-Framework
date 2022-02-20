#pragma once

#include "../Core/Layer.h"
#include "../Renderer/Shader.h"
#include "../Renderer/VertexArray.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Framebuffer.h"

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
	IndexBuffer*  m_IBO;
	VertexArray*  m_VAO;
	Framebuffer*  m_FBO;
	Shader*  m_Shader;
	Texture* m_Texture;
	bool m_Show_demo_window;
};