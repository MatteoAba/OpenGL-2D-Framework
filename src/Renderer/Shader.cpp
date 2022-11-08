#include "Shader.h"
#include "../Core/Log.h"
#include <glad/glad.h>

Shader::Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
    : m_Name(name), m_VertexPath(vertexPath), m_FragmentPath(fragmentPath)
{
    LOG_TRACE("Loading shader from {} and {}", vertexPath, fragmentPath);

    // ricavo i sorgenti di vertex e fragment dai rispettivi file
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // mi assicuro che gli ifstream possano lanciare delle eccezioni
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // apro i file
        vShaderFile.open(vertexPath.c_str());
        fShaderFile.open(fragmentPath.c_str());
        std::stringstream vShaderStream, fShaderStream;

        // leggo i buffer contenuti negli stream
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // chiudo i file handlers
        vShaderFile.close();
        fShaderFile.close();

        // converto gli stream in stringhe
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        // i file non sono stati letti
        LOG_ERROR("Shader files not found");
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    uint32_t vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    m_ShaderID = glCreateProgram();
    glAttachShader(m_ShaderID, vertex);
    glAttachShader(m_ShaderID, fragment);
    glLinkProgram(m_ShaderID);
    checkCompileErrors(m_ShaderID, "PROGRAM");

    // elimino gli shader sorgenti
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderID);
}

void Shader::Bind()
{
	glUseProgram(m_ShaderID);
}

void Shader::Unbind()
{
	glUseProgram(m_ShaderID);
}

void Shader::checkCompileErrors(uint32_t shader, const std::string& type)
{
    GLint success;
    char infoLog[1024];

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            LOG_ERROR("[{}] Shader compilation failed", type);
            LOG_ERROR("[INFO] : {}", infoLog);
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            LOG_ERROR("[{}] Shader linking fallito", type);
            LOG_ERROR("[INFO] : {}", infoLog);
        }
    }
}

void Shader::SetInt(const std::string& name, int value)
{
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniform1i(location, value);
}

void Shader::SetIntArray(const std::string& name, int* value, uint32_t count)
{
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniform1iv(location, count, value);
}

void Shader::SetFloat(const std::string& name, float value)
{
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniform1f(location, value);
}

void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
{
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniform2f(location, value.x, value.y);
}

void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
{
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
{
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::SetMat3(const std::string& name, glm::mat3& matrix)
{
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix)
{
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}
