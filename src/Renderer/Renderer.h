#pragma once

#include "../Core/Application.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Framebuffer.h"

struct RendererStats
{
	float frameTime;
	float eventTime;
	float updateTime;
	float renderTime;
	uint32_t drawCall;

	RendererStats(float v0, float v1, float v2, float v3, uint32_t v4)
		: frameTime(v0), eventTime(v1), updateTime(v2), renderTime(v3), drawCall(v4) {}
};

class Renderer
{
public:
	// configuration
	static void Init(Application* owner, int maxFrameRate = 0);
	static void SetBlending(bool value);
	static void ActivateDepthTesting(bool value);
	static void SetMaxFramerate(int maxFrameRate);
	static inline float GetMaxframetime() { return m_MaxFrameTime; }
	static void SetVSync(bool value);
	static inline bool GetVSync() { return m_VSync; }
	static void SetMSAA(bool value);
	static inline bool GetMSAA() { return m_MSAA; }

	// rendering
	static void SetClearColor(float r, float g, float b, float a);
	static void ClearScreen();
	static void DrawQuad(VertexArray* VAO, IndexBuffer* IBO, Shader* Shader, Framebuffer* FBO = nullptr);
	
	// statistics
	static inline RendererStats GetStats() { return m_RenderStats; }
	static inline void SetFrameTime(float value)   { m_RenderStats.frameTime  = value; }
	static inline void SetEventTime(float value)   { m_RenderStats.eventTime  = value; }
	static inline void SetUpdateTime(float value)  { m_RenderStats.updateTime = value; }
	static inline void SetRenderTime(float value)  { m_RenderStats.renderTime = value; }
	static inline void SetDrawCall(uint32_t value) { m_RenderStats.drawCall   = value; }
	static inline void IncrementDrawCall()         { ++m_RenderStats.drawCall; }

private:
	static Application*  m_Owner;
	static RendererStats m_RenderStats;
	static glm::vec4 m_ClearColor;
	static bool m_Blendig;
	static bool m_DepthTesting;
	static bool m_VSync;
	static bool m_MSAA;
	static float m_MaxFrameTime;
};