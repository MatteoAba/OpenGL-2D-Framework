#pragma once

#include "../Core/Application.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "Texture.h"

struct RendererStats
{
	float frameTime;
	float eventTime;
	float updateTime;
	float renderTime;
	uint32_t drawCall;
	uint32_t renderedQuad;

	RendererStats(float v0, float v1, float v2, float v3, uint32_t v4, uint32_t v5)
		: frameTime(v0), eventTime(v1), updateTime(v2), renderTime(v3), drawCall(v4), renderedQuad(v5) {}
};

struct Vertex2D
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec2 textureCoordinates;
	float textureSlotID;

	Vertex2D() {}
	Vertex2D(glm::vec3 pos, glm::vec4 col, glm::vec2 texCoord, float texID)
		: position(pos), color(col), textureCoordinates(texCoord), textureSlotID(texID) {}
	void Print();
};

struct BatchData
{
	VertexBuffer* VBO;
	IndexBuffer*  IBO;
	VertexArray*  VAO;
	TextureArray* textures;
	uint32_t indicesToDraw;
	Vertex2D *buffer, *nextVertexPosition;

	BatchData(uint32_t maxVerticesCount, uint32_t maxIndicesCount, glm::u32vec3 textureArraySizes);
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
	static void DrawQuad(VertexArray* VAO, Shader* Shader, Framebuffer* FBO = nullptr);
	static void DrawQuad(VertexArray* VAO, IndexBuffer* IBO, Shader* Shader, Framebuffer* FBO = nullptr, uint32_t indicesCount = 6);
	
	// batch rendering
	static void SetupBatchRendering(uint32_t maxQuadsCount, Shader* shader, glm::u32vec3 textureArraySizes, Framebuffer* FBO);
	static void StopBatchRendering();
	static void InitNewBatch();
	static void EndBatch();
	static float AddTextureToBatch(const std::string& filePath);
	static void DrawQuad(Vertex2D (&quad)[4]);

	// statistics
	static inline RendererStats GetStats() { return m_RenderStats; }
	static inline void SetFrameTime(float value)   { m_RenderStats.frameTime  = value; }
	static inline void SetEventTime(float value)   { m_RenderStats.eventTime  = value; }
	static inline void SetUpdateTime(float value)  { m_RenderStats.updateTime = value; }
	static inline void SetRenderTime(float value)  { m_RenderStats.renderTime = value; }
	static inline void SetDrawCall(uint32_t value) { m_RenderStats.drawCall   = value; }
	static inline void SetRenderedQuad(uint32_t value) { m_RenderStats.renderedQuad = value; }
	static inline void IncrementDrawCall()         { ++m_RenderStats.drawCall; }
	static inline void IncrementRenderedQuads(uint32_t value) { m_RenderStats.renderedQuad += value; }

private:
	static Application*  m_Owner;
	static RendererStats m_RenderStats;
	static glm::vec4 m_ClearColor;
	static bool m_Blendig;
	static bool m_DepthTesting;
	static bool m_VSync;
	static bool m_MSAA;
	static float m_MaxFrameTime;
	static uint32_t m_MaxTextureUnits, m_MaxArrayTextureLayers;

	// batch rendering
	static uint32_t m_MaxQuadsCount, m_MaxVerticesCount, m_MaxIndicesCount;
	static BatchData* m_BatchData;
	static Shader* m_BatchShader;
	static Framebuffer* m_BatchFBO;
};