#pragma once

#include "../Core/Layer.h"
#include "../Renderer/Shader.h"
#include "../Renderer/VertexArray.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Framebuffer.h"
#include "../Renderer/OrthographicCamera.h"
#include "../Scene/Entity.h"
#include "../Scene/Component/SpriteComponent.h"
#include "../Scene/Component/InputComponent.h"
#include "../Scene/Component/CameraComponent.h"

class TilesMapLayer : public Layer
{
public:
	TilesMapLayer(std::string name, Application* owner);

	// creation and destruction in application
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	// application loop behaviour
	virtual void OnEvent(Event e) override;
	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;
	virtual void OnImGuiRender() override;

private:
	Framebuffer* m_FBO;
	Shader* m_Shader;
	OrthographicCamera* m_Camera;
	glm::u32vec2** m_TerrainMap;
	glm::u32vec2 m_SmallHouse[5][4];
	uint32_t m_TerrainMapWidth, m_TerrainMapHeight, m_QuadSize;
	Entity* m_Entity;
	SpriteComponent* m_SpriteComponent;
};