#include "Renderer.h"

Application* Renderer::m_Owner = nullptr;
RendererStats Renderer::m_RenderStats(0.0f, 0.0f, 0.0f, 0.0f, 0);
glm::vec4 Renderer::m_ClearColor = {0.0f, 0.0f, 0.0f, 1.0f};
bool Renderer::m_Blendig = false;
bool Renderer::m_DepthTesting = false;
float Renderer::m_MaxFrameTime = 0.0f;
bool Renderer::m_VSync = false;
bool Renderer::m_MSAA = false;

void Renderer::Init(Application* owner, int maxFrameRate)
{
	m_Owner = owner;
	if (maxFrameRate)
		m_MaxFrameTime = 1.0f / (float)maxFrameRate;
	else
		m_MaxFrameTime = 0.0f;
}

void Renderer::SetBlending(bool value)
{
	if (value && !m_Blendig) {
		m_Blendig = true;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else if (!value && m_Blendig) {
		m_Blendig = false;
		glDisable(GL_BLEND);
	}
}

void Renderer::ActivateDepthTesting(bool value)
{
	if (value && !m_DepthTesting) {
		m_DepthTesting = true;
		glEnable(GL_DEPTH_TEST);
	}
	else if (value && !m_DepthTesting) {
		m_DepthTesting = false;
		glDisable(GL_DEPTH_TEST);
	}
}

void Renderer::SetMaxFramerate(int maxFrameRate)
{
	if (maxFrameRate) {
		m_MaxFrameTime = 1.0f / (float)maxFrameRate;
		LOG_INFO("Framerate limit set to {}fps", maxFrameRate);
	}
	else {
		m_MaxFrameTime = 0.0f;
		LOG_WARN("Framerate unlocked");
	}
}

void Renderer::SetVSync(bool value)
{
	if (value && !m_VSync) {
		m_VSync = true;
		m_Owner->GetWindow()->SetVSync(1);
	} 
	else if (!value && m_VSync) {
		m_VSync = false;
		m_Owner->GetWindow()->SetVSync(0);
	}
}

void Renderer::SetMSAA(bool value)
{
	if (value && !m_MSAA) {
		m_MSAA = true;
		m_Owner->GetWindow()->SetAntiAliasingMSAA(4);
	}
	else if (!value && m_MSAA) {
		m_MSAA = false;
		m_Owner->GetWindow()->SetAntiAliasingMSAA(0);
	}
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
	m_ClearColor = { r, g, b, a };
	glClearColor(r, g, b, a);
}

void Renderer::ClearScreen()
{
	if (m_DepthTesting)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else
		glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawQuad(VertexArray* VAO, Shader* Shader, Framebuffer* FBO)
{
	// framebuffer binding
	if (FBO)
		FBO->Bind();
	
	// buffers binding
	Shader->Bind();
	VAO->Bind();

	// draw call
	glDrawArrays(GL_TRIANGLES, 0, 6);
	Renderer::IncrementDrawCall();

	// unbinding
	Shader->Unbind(); 
	VAO->Unbind();
	
	// binding default framebuffer
	if (FBO)
		FBO->Unbind();
}

void Renderer::DrawQuad(VertexArray* VAO, IndexBuffer* IBO, Shader* Shader, Framebuffer* FBO)
{
	// framebuffer binding
	if (FBO)
		FBO->Bind();
	
	// buffers binding
	Shader->Bind();
	VAO->Bind();
	IBO->Bind();

	// draw call
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	Renderer::IncrementDrawCall();

	// unbinding
	Shader->Unbind(); 
	VAO->Unbind();
	IBO->Unbind();
	
	// binding default framebuffer
	if (FBO)
		FBO->Unbind();
}
