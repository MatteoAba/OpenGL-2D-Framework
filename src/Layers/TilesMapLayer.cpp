#include "TilesMapLayer.h"

#include "../cglph.h"
#include "../Core/Log.h"
#include "../Core/Application.h"
#include "../Renderer/Renderer.h"

glm::vec4 getUVCoordinates(uint32_t i, uint32_t j, uint32_t subTextureWidth, uint32_t subTextureHeight, uint32_t textureWidth, uint32_t textureHeight);

TilesMapLayer::TilesMapLayer(std::string name, Application* owner)
	: Layer(name, owner)
{
}

void TilesMapLayer::OnAttach()
{
	// camera e controller
	float width = (float)(m_Owner->GetWindow()->GetViewportWidth());
	float height = (float)(m_Owner->GetWindow()->GetViewportHeight());
	m_Camera = new OrthographicCamera(width, height);
	m_CameraController = new OrthographicCameraController(m_Camera, 500.0f);

	// shader
	m_Shader = new Shader("Quad", "assets/Shader/Vertex2D.vert", "assets/Shader/Vertex2D.frag");
	m_Shader->Bind();
	m_Shader->SetInt("u_Textures", 0);
	m_Shader->SetMat4("u_Model", glm::mat4(1.0f));
	m_Shader->Unbind();

	// framebuffer
	m_FBO = new Framebuffer(m_Owner, true);

	// batch renderer
	uint32_t maxVerticesCount = 1000;
	glm::u32vec3 textureArraySize = { 208, 160, 2 };						// { width, height, layers number }
	Renderer::SetupBatchRendering(maxVerticesCount, m_Shader, textureArraySize, m_FBO);
	Renderer::AddTextureToBatch("assets/Img/tileset.png");
	Renderer::SetBlending(true);

	// terrain map
	glm::u32vec2 terrainMap[3][3] = {
		{ glm::u32vec2(7, 0), glm::u32vec2(7, 1), glm::u32vec2(7, 2) },
		{ glm::u32vec2(8, 0), glm::u32vec2(8, 1), glm::u32vec2(8, 2) },
		{ glm::u32vec2(9, 0), glm::u32vec2(9, 1), glm::u32vec2(9, 2) }
	};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m_TerrainMap[i][j][0] = terrainMap[i][j][0];
			m_TerrainMap[i][j][1] = terrainMap[i][j][1];
		}
	}
}

void TilesMapLayer::OnDetach()
{
	// stop batch renderer
	Renderer::StopBatchRendering();

	delete m_Shader;
	delete m_FBO;
	delete m_Camera;
	delete m_CameraController;
}

void TilesMapLayer::OnEvent(Event e)
{
	m_CameraController->OnEvent(e);
}

void TilesMapLayer::OnUpdate(float ts)
{
	m_CameraController->OnUpdate(ts);
}

void TilesMapLayer::OnRender()
{
	m_FBO->Bind();
	Renderer::ClearScreen();
	m_FBO->Unbind();

	// quads submissions
	Renderer::InitNewBatch();
	float quadSize = 100.0f;
	LOG_WARN("START");
	for (uint32_t i = 0; i < 3; i++) {
		for (uint32_t j = 0; j < 3; j++) {
			// position options
			float x = j * quadSize;
			float y = i * quadSize;

			// texture and color options
			float textureSlotID = 1.0f;
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

			// texture atlas u/v
			glm::vec4 uvCoordinates = getUVCoordinates(m_TerrainMap[i][j][0], m_TerrainMap[i][j][1], 16, 16, 208, 160);
			// glm::vec4 uvCoordinates = {0.0f, 0.0f, 1.0f, 1.0f};

			// quad generation
			Vertex2D v0({ x + quadSize, y + quadSize, 0.0f }, color, { uvCoordinates[2], uvCoordinates[3]}, textureSlotID);     		// right - up
			Vertex2D v1({ x + quadSize, y, 0.0f }, color, { uvCoordinates[2], uvCoordinates[1] }, textureSlotID);       						// right - down
			Vertex2D v2({ x, y, 0.0f }, color, { uvCoordinates[0], uvCoordinates[1] }, textureSlotID);       											// left  - down
			Vertex2D v3({ x, y + quadSize, 0.0f }, color, { uvCoordinates[0], uvCoordinates[3] }, textureSlotID);       						// left  - up
			Vertex2D quad[4] = { v0, v1, v2, v3 };
			
			// add quad to batch
			Renderer::DrawQuad(quad);
		}
	}
	LOG_WARN("STOP\n");

	// quads rendering
	m_Shader->Bind();
	m_Shader->SetMat4("u_View", m_Camera->GetView());
	m_Shader->SetMat4("u_Projection", m_Camera->GetProjection());
	m_Shader->Unbind();
	Renderer::EndBatch();
}

void TilesMapLayer::OnImGuiRender()
{
	ImGui::Begin("Viewport");
	{
		ImVec2 wsize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)(uint64_t)m_FBO->GetColorAttachment(), wsize, ImVec2(0, 1), ImVec2(1, 0));

		// update viewport
		m_Owner->GetWindow()->UpdateViewport((uint32_t)wsize.x, (uint32_t)wsize.y);
	}
	ImGui::End();
}

glm::vec4 getUVCoordinates(uint32_t i, uint32_t j, uint32_t subTextureWidth, uint32_t subTextureHeight, uint32_t textureWidth, uint32_t textureHeight)
{
	float x0 = float(j * subTextureWidth) / (float)textureWidth;
	float y0 = float(i * subTextureHeight) / (float)textureHeight;
	float x1 = float((j + 1) * subTextureWidth) / (float)textureWidth;
	float y1 = float((i + 1) * subTextureHeight) / (float)textureHeight;
	LOG_WARN("{} {} {} {} {} {}", i, j, subTextureWidth, subTextureHeight, textureWidth, textureHeight);
	LOG_WARN("x0={} y0={} x1={} y1={}", x0, y0, x1, y1);

	return { x0, y0, x1, y1 };
}
