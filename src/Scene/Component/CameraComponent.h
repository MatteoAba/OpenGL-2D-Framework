#pragma once

#include "../Component.h"
#include "../../Renderer/OrthographicCamera.h"

class CameraComponent : public Component
{
public:
	CameraComponent(Entity* owner, OrthographicCamera* camera, uint32_t updateOrder = 100)
		: Component(owner, updateOrder), m_Camera(camera) {};
	~CameraComponent() {}

	virtual void OnEvent(Event e) override;
	virtual void OnUpdate(float ts) override;

private:
	 OrthographicCamera* m_Camera;
};
