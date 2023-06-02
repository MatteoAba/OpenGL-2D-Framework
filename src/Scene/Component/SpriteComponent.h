#pragma once

#include "../Component.h"
#include "../../Core/Event.h"

class SpriteComponent : public Component
{
public:
	SpriteComponent(Entity* owner, float textureArrayLayer, glm::vec2 spriteSize, glm::vec2 textureSize, glm::vec2 uvCoordinates, uint32_t updateOrder = 100)
		: Component(owner, updateOrder), m_TextureArrayLayer(textureArrayLayer), m_SpriteSize(spriteSize), m_TextureSize(textureSize), m_UVCoordinates(uvCoordinates) {};
	~SpriteComponent() {}

	virtual void OnEvent(Event e) override;
	virtual void OnUpdate(float ts)override;
	void OnRender();

private:
	float m_TextureArrayLayer;
	glm::vec2 m_SpriteSize;
	glm::vec2 m_TextureSize;
	glm::vec2 m_UVCoordinates;
};