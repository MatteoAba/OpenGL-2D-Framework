#include <glad/glad.h>

#include "../Core/Log.h"
#include "TestLayer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	// falso se una funzione OpenGL precedente non ha avuto successo
	while (GLenum error = glGetError()) {
		//stampo l'errore
		LOG_ERROR("[OpenGL] ({}) {} - {} - {} ", error, function, file, line);
		return false;
	}
	return true;
}

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void TestLayer::OnAttach()
{
	// <------ TRIANGOLO ------>

	// shader per il triangolo
	m_Shader = new Shader("Triangolo", "assets/Shader/triangolo.vert", "assets/Shader/triangolo.frag");

	// vertices
	float vertices[] = {
		// positions            // colors
		 0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f 
	};

	// vertex buffer
	GLCall(glGenBuffers(1, &m_VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	// vertex array
	GLCall(glGenVertexArrays(1, &m_VAO));
	GLCall(glBindVertexArray(m_VAO));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));

	// <------ FINE TRIANGOLO ------>

	// <------ FRAMEBUFFER ------>

	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// creo il color attachment
	glGenTextures(1, &m_TCB);
	glBindTexture(GL_TEXTURE_2D, m_TCB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TCB, 0);

	// creo il renderbuffer per il depth e stencil attachment
	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);                 // 24 bit per depth e 8 bit per stencil
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

	// controllo se il frame buffer è completo
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG_ERROR("FRAMEBUFFER INCOMPLETO");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// <------ FINE FRAMEBUFFER ------>

	// <-------- QUAD --------->
	
	// renderizzo un quad che copre tutto lo schermo
	float quad[] = {
		// positions       // texCoords
		-1.0f,  1.0f,      0.0f, 1.0f,
		-1.0f, -1.0f,      0.0f, 0.0f,
		 1.0f, -1.0f,      1.0f, 0.0f,

		-1.0f,  1.0f,      0.0f, 1.0f,
		 1.0f, -1.0f,      1.0f, 0.0f,
		 1.0f,  1.0f,      1.0f, 1.0f
	};

	// shader che stampa sul quad il framebuffer (come un texture)
	m_ScreenShader = new Shader("Screen Shader", "assets/Shader/TextureOnQuad.vert", "assets/Shader/TextureOnQuad.frag");

	// vertex buffer
	GLCall(glGenBuffers(1, &m_VBO_Screen));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO_Screen));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW));

	// vertex array
	GLCall(glGenVertexArrays(1, &m_VAO_Screen));
	GLCall(glBindVertexArray(m_VAO_Screen));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));

	// <------ FINE QUAD ------>
}

void TestLayer::OnDetach()
{
	// deallocazione shader
	delete m_Shader;
	delete m_ScreenShader;

	// deallocazione vertex array
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteVertexArrays(1, &m_VAO_Screen);

	// deallocazione vertex buffer
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_VBO_Screen);

	// deallocazione framebuffer
	glDeleteFramebuffers(1, &m_FBO);
}

void TestLayer::OnEvent(Event e)
{
}

void TestLayer::OnUpdate(float ts)
{
}

void TestLayer::OnRender()
{
	// rendering del triangolo nel Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	m_Shader->Bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	GLCall(glBindVertexArray(m_VAO));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
	m_Shader->Unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// rendering del piano
	m_ScreenShader->Bind();
	GLCall(glBindVertexArray(m_VAO_Screen));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_TCB));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));
	m_ScreenShader->Unbind();
}
