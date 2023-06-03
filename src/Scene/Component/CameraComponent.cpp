#include "CameraComponent.h"
#include "../Entity.h"

void CameraComponent::OnEvent(Event e)
{
	// if the viewport size change, the projection camera should be updated
	if (e.GetType() == EventType::VIEWPORTRESIZE)
		m_Camera->ChangeProjection((float)e.GetStructure().Resize.width, (float)e.GetStructure().Resize.height);
}

void CameraComponent::OnUpdate(float ts)
{
	uint32_t width  = GetOwner()->GetOwner()->GetWindow()->GetViewportWidth();
	uint32_t height = GetOwner()->GetOwner()->GetWindow()->GetViewportHeight();

	// center the camera on the Entity <x,y> position
	glm::vec3 newPosition = {
		-GetOwner()->GetPosition().x + width / 2.0f,
		-GetOwner()->GetPosition().y + height / 2.0f,
		-3.0
	};

	// calculate new view matrix
	m_Camera->SetPosition(newPosition);
	m_Camera->TranslateCamera();
}
