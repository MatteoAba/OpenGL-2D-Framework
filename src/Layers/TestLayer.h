#pragma once

#include "../Core/Layer.h"
#include "../Renderer/Shader.h"
#include "../Renderer/VertexArray.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Framebuffer.h"
#include "../Renderer/OrthographicCamera.h"

class TestLayer : public Layer
{
public:
	TestLayer(std::string name, Application* owner);

	// creazione e distruzione
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	// loop di funzionamento
	virtual void OnEvent(Event e) override;
	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;

	float m_CameraSpeed = 10.0f;

private:
	VertexBuffer* m_VBO;
	IndexBuffer*  m_IBO;
	VertexArray*  m_VAO;
	Framebuffer*  m_FBO;
	Shader*  m_Shader;
	Texture* m_Texture;
	OrthographicCamera* m_Camera;
	OrthographicCameraController* m_CameraController;
	bool m_Show_demo_window;
};