#include "InputComponent.h"
#include "../Entity.h"

void InputComponent::OnEvent(Event e)
{
	// if a button is pressed the movement in that direction starts
	if (e.GetType() == EventType::KEYBTN && e.GetStructure().Key.action == KeyEventType::PRESSED) {
		switch (e.GetStructure().Key.button)
		{
		case Keycode::KEY_W:
			m_MovementDirection.y += 1.0f;
			break;
		case Keycode::KEY_A:
			m_MovementDirection.x -= 1.0f;
			break;
		case Keycode::KEY_S:
			m_MovementDirection.y -= 1.0f;
			break;
		case Keycode::KEY_D:
			m_MovementDirection.s += 1.0f;
			break;
		}
	}

	// if a button is released the movement in that direction stops
	if (e.GetType() == EventType::KEYBTN && e.GetStructure().Key.action == KeyEventType::RELEASED) {
		switch (e.GetStructure().Key.button)
		{
		case Keycode::KEY_W:
			m_MovementDirection.y -= 1.0f;
			break;
		case Keycode::KEY_A:
			m_MovementDirection.x += 1.0f;
			break;
		case Keycode::KEY_S:
			m_MovementDirection.y += 1.0f;
			break;
		case Keycode::KEY_D:
			m_MovementDirection.s -= 1.0f;
			break;
		}
	}
}

void InputComponent::OnUpdate(float ts)
{
	// apply movement
	glm::vec3 newPosition = {
		GetOwner()->GetPosition().x + (m_MovementSpeed * m_MovementDirection.x * ts),
		GetOwner()->GetPosition().y + (m_MovementSpeed * m_MovementDirection.y * ts),
		GetOwner()->GetPosition().z
	};
	GetOwner()->SetPosition(newPosition);
}
