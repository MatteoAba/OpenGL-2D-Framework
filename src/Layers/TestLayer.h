#pragma once

#include "../Core/Layer.h"
#include "../Renderer/Shader.h"
#include "../Renderer/VertexArray.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Framebuffer.h"
#include "../Renderer/OrthographicCamera.h"
#include "../Renderer/Text.h"

class TestLayer : public Layer
{
public:
	TestLayer(std::string name, Application* owner);

	// creation and destruction in application
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	// application loop behaviour
	virtual void OnEvent(Event e) override;
	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;
	virtual void OnImGuiRender() override;

private:
	VertexBuffer *m_VBO, *m_AtlasVBO;
	IndexBuffer* m_IBO;
	VertexArray  *m_VAO, *m_AtlasVAO;
	Framebuffer* m_FBO;
	Shader* m_Shader;
	Texture *m_Texture, *m_TextureAtlas;
	OrthographicCamera* m_Camera;
	OrthographicCameraController* m_CameraController;
	uint32_t m_AtlasRow, m_AtlasColumn;

	// text
	Text* m_Text;
	glm::vec2 m_TextPosition;
	float m_TextScale;
};