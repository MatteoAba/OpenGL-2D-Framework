#ifndef GL_DEBUG
#define GL_DEBUG

#include <glad/glad.h>
#include "../Core/Log.h"

// -------------------------------- OpenGL Debug ----------------------------------- //

#include "../Core/Log.h"

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

// -------------------------------------------------------------------------------- //

#endif