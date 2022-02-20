#pragma once

#include "../cglph.h"
#include <glad/glad.h>

class Application;

class Framebuffer
{
public:
	Framebuffer(Application* owner, bool createDepthAndStencil = false);
	~Framebuffer();
	inline void Bind()   { glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID); }
	inline void Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
	inline uint32_t GetColorAttachment() { return m_ColorAttacmentID; }

private:
	void CreateDepthAndStencil();

private:
	uint32_t m_RendererID;
	uint32_t m_ColorAttacmentID;					// texture
	uint32_t m_DepthAndStencilAttachmentID;			// render buffer (his sampling isn't required, is faster)
	Application* m_Owner;
};