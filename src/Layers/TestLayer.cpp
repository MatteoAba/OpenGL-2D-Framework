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
		std::cout << "[ERRORE | OpenGL] (" << error << ") : " << function << " " << file << " " << line << std::endl;
		return false;
	}
	return true;
}

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void TestLayer::OnAttach()
{
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
	GLCall(glGenVertexArrays(1, &m_VAO));
	GLCall(glBindVertexArray(m_VAO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	// vertex array
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(1));
}

void TestLayer::OnDetach()
{
	delete m_Shader;
}

void TestLayer::OnEvent(Event e)
{
}

void TestLayer::OnUpdate(float ts)
{
}

void TestLayer::OnRender()
{
	m_Shader->Bind();
	GLCall(glBindVertexArray(m_VAO));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}
