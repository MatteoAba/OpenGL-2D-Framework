#pragma once

#include "../cglph.h"
#include "../Core/Event.h"

class OrthographicCamera
{
public:
	OrthographicCamera(float width, float height);
	~OrthographicCamera() {}

	void TranslateCamera(glm::vec3 translation);
	void ChangeProjection(float width, float height);

	inline glm::mat4 GetView() { return m_View; }
	inline glm::mat4 GetProjection() { return m_Projection; }

private:
	glm::mat4 m_View;
	glm::mat4 m_Projection;
};

class OrthographicCameraController
{
public:
	OrthographicCameraController(OrthographicCamera* camera, float cameraSpeed = 100.0f);
	~OrthographicCameraController() {}
	
	void OnEvent(Event e);
	void OnUpdate(float ts);

	inline float* GetSpeedPointer() { return &m_CameraSpeed; }

private:
	float m_CameraSpeed;
	glm::vec3 m_Movement;
	OrthographicCamera* m_Camera;
};