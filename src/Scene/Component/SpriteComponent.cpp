#include "SpriteComponent.h"
#include "../../Renderer/Renderer.h"
#include "../Entity.h"

void SpriteComponent::OnEvent(Event e)
{

}

void SpriteComponent::OnUpdate(float ts)
{

}

void SpriteComponent::OnRender()
{
	// position options
	float x = Component::GetOwner()->GetPosition().x - m_SpriteSize[0] / 2;
	float y = Component::GetOwner()->GetPosition().y - m_SpriteSize[1] / 2;
	glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	// quad generation
	Vertex2D v0({ x + m_SpriteSize[0], y + m_SpriteSize[1], 0.0f }, color, { m_UVCoordinates[0] + m_TextureSize[0], m_UVCoordinates[1] + m_TextureSize[1] }, m_TextureArrayLayer);     		// right - up
	Vertex2D v1({ x + m_SpriteSize[0], y, 0.0f }, color, { m_UVCoordinates[0] + m_TextureSize[0], m_UVCoordinates[1] }, m_TextureArrayLayer);       										// right - down
	Vertex2D v2({ x, y, 0.0f }, color, { m_UVCoordinates[0], m_UVCoordinates[1] }, m_TextureArrayLayer);       																				// left  - down
	Vertex2D v3({ x, y + m_SpriteSize[1], 0.0f }, color, { m_UVCoordinates[0],  m_UVCoordinates[1] + m_TextureSize[1] }, m_TextureArrayLayer);       										// left  - up
	Vertex2D quad[4] = { v0, v1, v2, v3 };

	// add quad to batch
	Renderer::DrawQuad(quad);
}