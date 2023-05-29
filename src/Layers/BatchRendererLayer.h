#pragma once

#include "../Core/Layer.h"
#include "../Renderer/Shader.h"
#include "../Renderer/VertexArray.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Framebuffer.h"
#include "../Renderer/OrthographicCamera.h"

class BatchRendererLayer : public Layer
{
public:
	BatchRendererLayer(std::string name, Application* owner);

	// creation and destruction in application
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	// application loop behaviour
	virtual void OnEvent(Event e) override;
	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;
	virtual void OnImGuiRender() override;

private:
	Framebuffer*  m_FBO;
	Shader* m_Shader;
	OrthographicCamera* m_Camera;
	OrthographicCameraController* m_CameraController;
	uint8_t m_CheckerboardRows, m_CheckerboardColumns;
	float m_QuadSize;
	std::vector<glm::vec3> m_TextureInfo;
};