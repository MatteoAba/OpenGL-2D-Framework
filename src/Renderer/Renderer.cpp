#include "Renderer.h"

// static members initialization
Application* Renderer::m_Owner = nullptr;
RendererStats Renderer::m_RenderStats(0.0f, 0.0f, 0.0f, 0.0f, 0, 0);
glm::vec4 Renderer::m_ClearColor = {0.0f, 0.0f, 0.0f, 1.0f};
bool Renderer::m_Blendig = false;
bool Renderer::m_DepthTesting = false;
float Renderer::m_MaxFrameTime = 0.0f;
bool Renderer::m_VSync = false;
bool Renderer::m_MSAA = false;
uint32_t Renderer::m_MaxTextureUnits = 0;
uint32_t Renderer::m_MaxArrayTextureLayers = 0;
uint32_t Renderer::m_MaxQuadsCount = 1000; 
uint32_t Renderer::m_MaxVerticesCount = 1000 * 4;
uint32_t Renderer::m_MaxIndicesCount = 1000 * 6;
BatchData* Renderer::m_BatchData = nullptr;
Shader* Renderer::m_BatchShader = nullptr;
Framebuffer* Renderer::m_BatchFBO = nullptr;

void Vertex2D::Print()
{
	LOG_TRACE("Position: {} {} {}", position.x, position.y, position.z);
	LOG_TRACE("Color: {} {} {}", color.r, color.g, color.b, color.a);
	LOG_TRACE("Texture Coordinates: {} {}", textureCoordinates.x, textureCoordinates.y);
	LOG_TRACE("Texture Layer: {}", textureSlotID);
}

BatchData::BatchData(uint32_t maxVerticesCount, uint32_t maxIndicesCount, glm::u32vec3 textureArraySizes)
{
	// vertex buffer
	VBO = new VertexBuffer(maxVerticesCount * sizeof(Vertex2D));

	// index buffer
	uint32_t* indices = new uint32_t[maxIndicesCount];
	for (uint32_t i = 0, offset = 0; i < maxIndicesCount; i += 6, offset += 4) {
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;
		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;
	}
	IBO = new IndexBuffer((void*)indices, maxIndicesCount * sizeof(uint32_t));
	delete[] indices;
	indicesToDraw = 0;

	// vertex array
	VAO = new VertexArray();
	VertexLayout layout;
	layout.AddVertexGroup({ DataType::CGL_FLOAT, 3, false });
	layout.AddVertexGroup({ DataType::CGL_FLOAT, 4, false });
	layout.AddVertexGroup({ DataType::CGL_FLOAT, 2, false });
	layout.AddVertexGroup({ DataType::CGL_FLOAT, 1, false });
	VAO->AddBuffer(layout, *VBO);

	// textures
	textures = new TextureArray(textureArraySizes[0], textureArraySizes[1], textureArraySizes[2]);
	textures->Bind();
	textures->SubmitTexture();							// white texture

	// quads CPU buffer
	buffer = new Vertex2D[maxVerticesCount];
}

void Renderer::Init(Application* owner, int maxFrameRate)
{
	m_Owner = owner;

	// framerate
	if (maxFrameRate)
		m_MaxFrameTime = 1.0f / (float)maxFrameRate;
	else
		m_MaxFrameTime = 0.0f;

	// hardware specific values
	int maxTextureUnit, maxArrayTextureLayers;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxTextureUnit);
	glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &maxArrayTextureLayers);
	m_MaxTextureUnits = (uint32_t)maxTextureUnit;
	m_MaxArrayTextureLayers = (uint32_t)maxArrayTextureLayers;
	LOG_INFO("Number of Texture Unit: {}", m_MaxTextureUnits);
	LOG_INFO("Maximum Number of Layers for Texture Units: {}", m_MaxArrayTextureLayers);
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
	Renderer::IncrementRenderedQuads(1);

	// unbinding
	Shader->Unbind(); 
	VAO->Unbind();
	
	// binding default framebuffer
	if (FBO)
		FBO->Unbind();
}

void Renderer::DrawQuad(VertexArray* VAO, IndexBuffer* IBO, Shader* Shader, Framebuffer* FBO, uint32_t indicesCount)
{
	// framebuffer binding
	if (FBO)
		FBO->Bind();
	
	// buffers binding
	Shader->Bind();
	VAO->Bind();
	IBO->Bind();

	// draw call
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
	Renderer::IncrementDrawCall();
	Renderer::IncrementRenderedQuads(indicesCount / 6);

	// unbinding
	Shader->Unbind(); 
	VAO->Unbind();
	IBO->Unbind();
	
	// binding default framebuffer
	if (FBO)
		FBO->Unbind();
}

void Renderer::SetupBatchRendering(uint32_t maxQuadsCount, Shader* shader, glm::u32vec3 textureArraySizes, Framebuffer* FBO)
{
	// setup limits
	m_MaxQuadsCount = maxQuadsCount;
	m_MaxVerticesCount = maxQuadsCount * 4;
	m_MaxIndicesCount = maxQuadsCount * 6;

	// setup Shader and Framebuffer target
	m_BatchShader = shader;
	m_BatchFBO = FBO;

	// start bacth data structure
	m_BatchData = new BatchData(m_MaxVerticesCount, m_MaxIndicesCount, textureArraySizes);
}

void Renderer::StopBatchRendering()
{
	// clean shader and FBO
	m_BatchShader = nullptr;
	m_BatchFBO = nullptr;

	// clean the batch data structure
	delete m_BatchData->VBO;
	delete m_BatchData->IBO;
	delete m_BatchData->VAO;
	delete m_BatchData->textures;
	delete[] m_BatchData->buffer;
	delete m_BatchData;
}

void Renderer::InitNewBatch() 
{
	// reset vertices buffer
	m_BatchData->nextVertexPosition = m_BatchData->buffer;
	m_BatchData->indicesToDraw = 0;
}

void Renderer::EndBatch() 
{
	// submit batched vertices to GPU
	GLsizeiptr size = (uint8_t*)m_BatchData->nextVertexPosition - (uint8_t*)m_BatchData->buffer;
	m_BatchData->VBO->SubmitData(m_BatchData->buffer, size);

	// draw call
	DrawQuad(m_BatchData->VAO, m_BatchData->IBO, m_BatchShader, m_BatchFBO, m_BatchData->indicesToDraw);
	m_BatchData->indicesToDraw = 0;
}


float Renderer::AddTextureToBatch(const std::string& filePath)
{
	uint32_t layer = m_BatchData->textures->SubmitTexture(filePath);
	return (float)layer;
}

void Renderer::DrawQuad(Vertex2D (&quad)[4]) 
{
	// check if there is space, else start a new batch
	if (m_BatchData->indicesToDraw == m_MaxIndicesCount) {
		EndBatch();
		InitNewBatch();
	}

	// copy the quad in the buffer
	memcpy(m_BatchData->nextVertexPosition, quad, sizeof(quad));
	m_BatchData->nextVertexPosition += 4;
	m_BatchData->indicesToDraw += 6;
}
