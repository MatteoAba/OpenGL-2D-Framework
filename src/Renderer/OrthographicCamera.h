#pragma once

#include "../cglph.h"
#include "../Core/Event.h"

class OrthographicCamera
{
public:
	OrthographicCamera(float width, float height, glm::vec3 position = { 0.0f, 0.0f, -3.0f });
	~OrthographicCamera() {}

	void TranslateCamera();
	void TranslateCamera(glm::vec3 translation);
	void ChangeProjection(float width, float height);

	// getters and setters
	inline glm::vec3 GetPosition() { return m_Position; }
	inline void SetPosition(const glm::vec3& position) { m_Position = position; }
	inline glm::mat4 GetView() { return m_View; }
	inline glm::mat4 GetProjection() { return m_Projection; }

private:
	glm::vec3 m_Position;
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