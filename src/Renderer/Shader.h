#pragma once

#include "../cglph.h"

class Shader
{
public:
	Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();
	void Bind();
	void Unbind();
	const std::string& GetName() { return m_Name; }
	const std::string& GetVertexPath() { return m_VertexPath; }
	const std::string& GetFragmentPath() { return m_FragmentPath; }

public:
	// gestione uniform			TODO: cache per la posizione
	void SetInt(const std::string& name, int value);
	void SetIntArray(const std::string& name, int* value, uint32_t count);
	void SetFloat(const std::string& name, float value);
	void SetFloat2(const std::string& name, const glm::vec2& value);
	void SetFloat3(const std::string& name, const glm::vec3& value);
	void SetFloat4(const std::string& name, const glm::vec4& value);
	void SetMat3(const std::string& name, glm::mat3& matrix);
	void SetMat4(const std::string& name, glm::mat4& matrix);

private:
	// utility
	void checkCompileErrors(uint32_t shader, const std::string& type);

private:
	std::string m_Name;
	std::string m_VertexPath, m_FragmentPath;
	uint32_t m_ShaderID;
};