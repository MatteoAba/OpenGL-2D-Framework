#pragma once

#include "../Core/Layer.h"
#include "../Renderer/Shader.h"

// TODO prendila dalla window (che va aggiornata acnhe con gli eventi)
#define SCR_WIDTH 640
#define SCR_HEIGHT 480

class TestLayer : public Layer
{
public:
	TestLayer(std::string name)
		: Layer(name) {}

	// creazione e distruzione
	virtual void OnAttach() override;
	virtual void OnDetach() override;

	// loop di funzionamento
	virtual void OnEvent(Event e) override;
	virtual void OnUpdate(float ts) override;
	virtual void OnRender() override;

private:
	uint32_t m_VAO, m_VBO, m_VAO_Screen, m_VBO_Screen;
	uint32_t m_FBO, m_TCB, m_RBO;
	Shader *m_Shader, *m_ScreenShader;
};