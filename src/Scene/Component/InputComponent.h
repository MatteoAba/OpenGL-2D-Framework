#pragma once

#include "../Component.h"

class InputComponent : public Component
{
public:
	InputComponent(Entity* owner, float movementSpeed, uint32_t updateOrder = 100)
		: Component(owner, updateOrder), m_MovementSpeed(movementSpeed), m_MovementDirection(glm::vec2(0.0f)) {};
	~InputComponent() {}

	virtual void OnEvent(Event e) override;
	virtual void OnUpdate(float ts) override;

private:
	float m_MovementSpeed;
	glm::vec2 m_MovementDirection;
};