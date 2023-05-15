#include "OrthographicCamera.h"
#include "../Core/Log.h"

OrthographicCamera::OrthographicCamera(float width, float height)
	: m_View(glm::mat4(1.0f))
{
	TranslateCamera(glm::vec3(0.0f, 0.0f, -0.5f));
	ChangeProjection(width, height);
}

void OrthographicCamera::TranslateCamera(glm::vec3 translation)
{
	m_View = glm::translate(m_View, translation);
}

void OrthographicCamera::ChangeProjection(float width, float height)
{
	// float aspectRatio = width / height;
	// m_Projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, 0.1f, 100.0f);
	m_Projection = glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
}

OrthographicCameraController::OrthographicCameraController(OrthographicCamera* camera, float cameraSpeed)
	: m_Camera(camera), m_CameraSpeed(cameraSpeed), m_Movement(glm::vec3(0.0f))
{
}

void OrthographicCameraController::OnEvent(Event e)
{
	// if a button is pressed the movement in that direction starts
	if (e.GetType() == EventType::KEYBTN && e.GetStructure().Key.action == KeyEventType::PRESSED) {
		switch (e.GetStructure().Key.button)
		{
		case Keycode::KEY_W:
			m_Movement += glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		case Keycode::KEY_A:
			m_Movement += glm::vec3(-1.0f, 0.0f, 0.0f);
			break;
		case Keycode::KEY_S:
			m_Movement += glm::vec3(0.0f, -1.0f, 0.0f);
			break;
		case Keycode::KEY_D:
			m_Movement += glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		}
	}

	// if a button is released the movement in that direction stops
	if (e.GetType() == EventType::KEYBTN && e.GetStructure().Key.action == KeyEventType::RELEASED) {
		switch (e.GetStructure().Key.button)
		{
		case Keycode::KEY_W:
			m_Movement += glm::vec3(0.0f, -1.0f, 0.0f);
			break;
		case Keycode::KEY_A:
			m_Movement += glm::vec3(1.0f, 0.0f, 0.0f);
			break;
		case Keycode::KEY_S:
			m_Movement += glm::vec3(0.0f, 1.0f, 0.0f);
			break;
		case Keycode::KEY_D:
			m_Movement += glm::vec3(-1.0f, 0.0f, 0.0f);
			break;
		}
	}

	// if the viewport size change, the projection camera should be updated
	if (e.GetType() == EventType::VIEWPORTRESIZE)
		m_Camera->ChangeProjection((float)e.GetStructure().Resize.width, (float)e.GetStructure().Resize.height);
}

void OrthographicCameraController::OnUpdate(float ts)
{
	if (m_Movement != glm::vec3(0.0f)) {
		m_Camera->TranslateCamera(m_Movement * m_CameraSpeed * ts);
	}
}
