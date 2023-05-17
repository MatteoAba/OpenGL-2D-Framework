#include "Framebuffer.h"
#include "../Core/Application.h"
#include "../Core/Log.h"

Framebuffer::Framebuffer(Application* owner, bool createDepthAndStencil)
	: m_Owner(owner), m_DepthAndStencilAttachmentID(0)
{
	glGenFramebuffers(1, &m_RendererID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

	// properties
	m_Properties = { m_Owner->GetWindow()->GetViewportWidth(), m_Owner->GetWindow()->GetViewportHeight() };

	// color attachment
	glGenTextures(1, &m_ColorAttacmentID);
	glBindTexture(GL_TEXTURE_2D, m_ColorAttacmentID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Owner->GetWindow()->GetViewportWidth(), m_Owner->GetWindow()->GetViewportHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttacmentID, 0);

	// depth and stencil buffer if needed
	if (createDepthAndStencil)
		CreateDepthAndStencil();

	// check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG_ERROR("Framebuffer not complete");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_RendererID);
}

void Framebuffer::Resize()
{
	uint32_t width  = m_Owner->GetWindow()->GetViewportWidth(); 
	uint32_t height = m_Owner->GetWindow()->GetViewportHeight();

	if (m_Properties.width != width || m_Properties.height != height) {
		// update old properties
		m_Properties.width  = width;
		m_Properties.height = height;
		glDeleteTextures(1, &m_ColorAttacmentID);

		// generate new attachment
		Bind();
		glGenTextures(1, &m_ColorAttacmentID);
		glBindTexture(GL_TEXTURE_2D, m_ColorAttacmentID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttacmentID, 0);

		// check if new framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			LOG_ERROR("Updated framebuffer not complete");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}

void Framebuffer::CreateDepthAndStencil()
{
	// il depth e lo stencil attachment li creo con dei renderbuffer (invece delle texture)
	glGenRenderbuffers(1, &m_DepthAndStencilAttachmentID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAndStencilAttachmentID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Owner->GetWindow()->GetViewportWidth(), m_Owner->GetWindow()->GetViewportHeight());                 // 24 bit per depth e 8 bit per stencil
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAndStencilAttachmentID);
}
