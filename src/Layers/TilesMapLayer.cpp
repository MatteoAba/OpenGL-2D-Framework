#include "TilesMapLayer.h"

#include "../oglph.h"
#include "../Core/Log.h"
#include "../Core/Application.h"
#include "../Renderer/Renderer.h"

glm::vec4 getUVCoordinates(uint32_t i, uint32_t j, uint32_t subTextureWidth, uint32_t subTextureHeight, uint32_t textureWidth, uint32_t textureHeight);

TilesMapLayer::TilesMapLayer(std::string name, Application* owner)
	: Layer(name, owner), m_TerrainMapWidth(10), m_TerrainMapHeight(7), m_QuadSize(100.0f)
{
}

void TilesMapLayer::OnAttach()
{
	// camera e controller
	float width = (float)(m_Owner->GetWindow()->GetViewportWidth());
	float height = (float)(m_Owner->GetWindow()->GetViewportHeight());
	m_Camera = new OrthographicCamera(width, height);

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

	// ------------ terrain map initialization ------------

	m_TerrainMap = new glm::u32vec2*[m_TerrainMapHeight];
	for (uint32_t i = 0; i < m_TerrainMapHeight; ++i)
		m_TerrainMap[i] = new glm::u32vec2[m_TerrainMapWidth];

	// internal quads
	for (uint32_t i = 1; i < m_TerrainMapHeight - 1; i++) {
		for (int j = 1; j < m_TerrainMapWidth - 1; j++) {
			m_TerrainMap[i][j][0] = 8;
			m_TerrainMap[i][j][1] = 1;
		}
	}

	// right and left borders
	for (uint32_t i = 1; i < m_TerrainMapHeight - 1; i++) {
		m_TerrainMap[i][0][0] = 8;
		m_TerrainMap[i][0][1] = 0;
		m_TerrainMap[i][m_TerrainMapWidth - 1][0] = 8;
		m_TerrainMap[i][m_TerrainMapWidth - 1][1] = 2;
	}

	// upper and lower borders
	for (uint32_t j = 1; j < m_TerrainMapWidth - 1; j++) {
		m_TerrainMap[0][j][0] = 7;
		m_TerrainMap[0][j][1] = 1;
		m_TerrainMap[m_TerrainMapHeight - 1][j][0] = 9;
		m_TerrainMap[m_TerrainMapHeight - 1][j][1] = 1;
	}

	// corners
	m_TerrainMap[0][0][0] = 7;
	m_TerrainMap[0][0][1] = 0;
	m_TerrainMap[0][m_TerrainMapWidth - 1][0] = 7;
	m_TerrainMap[0][m_TerrainMapWidth - 1][1] = 2;
	m_TerrainMap[m_TerrainMapHeight - 1][0][0] = 9;
	m_TerrainMap[m_TerrainMapHeight - 1][0][1] = 0;
	m_TerrainMap[m_TerrainMapHeight - 1][m_TerrainMapWidth - 1][0] = 9;
	m_TerrainMap[m_TerrainMapHeight - 1][m_TerrainMapWidth - 1][1] = 2;

	// ----------------------------------------------------

	// ------------ small house initialization ------------

	glm::u32vec2 smallHouse[5][4] = {
		{ { 2.0f, 0.0f }, { 2.0f, 1.0f }, { 2.0f, 2.0f }, { 2.0f, 3.0f } },
		{ { 3.0f, 0.0f }, { 3.0f, 1.0f }, { 3.0f, 2.0f }, { 3.0f, 3.0f } },
		{ { 4.0f, 0.0f }, { 4.0f, 1.0f }, { 4.0f, 2.0f }, { 4.0f, 3.0f } },
		{ { 5.0f, 0.0f }, { 5.0f, 1.0f }, { 5.0f, 2.0f }, { 5.0f, 3.0f } },
		{ { 6.0f, 0.0f }, { 6.0f, 1.0f }, { 6.0f, 2.0f }, { 6.0f, 3.0f } }
	};

	for (uint32_t i = 0; i < 5; i++) {
		for (int j = 0; j < 4; j++) {
			m_SmallHouse[i][j][0] = smallHouse[i][j][0];
			m_SmallHouse[i][j][1] = smallHouse[i][j][1];
		}
	}

	// ----------------------------------------------------

	// entity and components
	m_Entity = new Entity(m_Owner);
	m_Entity->SetPosition({200.0f, 200.0f, 0.0f});
	glm::vec2 spriteSize  = { 100.0f, 100.0f }; 
	glm::vec2 textureSize = { 16.0f / 208.0f, 16.0f / 160.0f };
	glm::vec4 uvCoordinates = getUVCoordinates(1.0f, 0.0f, 16, 16, 208, 160);
	m_SpriteComponent = new SpriteComponent(m_Entity, 1.0f, spriteSize, textureSize, uvCoordinates);
	m_Entity->AddComponent(m_SpriteComponent);
	InputComponent* inputComponent = new InputComponent(m_Entity, 300.0f);
	m_Entity->AddComponent(inputComponent);
	CameraComponent* cameraComponent = new CameraComponent(m_Entity, m_Camera);
	m_Entity->AddComponent(cameraComponent);
}

void TilesMapLayer::OnDetach()
{
	// stop batch renderer
	Renderer::StopBatchRendering();

	for (uint32_t i = 0; i < m_TerrainMapHeight; ++i)
		delete[] m_TerrainMap[i];
	delete[] m_TerrainMap;

	delete m_Shader;
	delete m_FBO;
	delete m_Camera;
	delete m_Entity;
}

void TilesMapLayer::OnEvent(Event e)
{
	m_Entity->OnEvent(e);
}

void TilesMapLayer::OnUpdate(float ts)
{
	m_Entity->OnUpdate(ts);
}

void TilesMapLayer::OnRender()
{
	m_FBO->Bind();
	Renderer::ClearScreen();
	m_FBO->Unbind();
	Renderer::InitNewBatch();

	// terrain quads submissions
	for (uint32_t i = 0; i < m_TerrainMapHeight; i++) {
		for (uint32_t j = 0; j < m_TerrainMapWidth; j++) {
			// position options
			float x = j * m_QuadSize;
			float y = i * m_QuadSize;

			// texture and color options
			float textureSlotID = 1.0f;
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

			// texture atlas u/v
			glm::vec4 uvCoordinates = getUVCoordinates(m_TerrainMap[i][j][0], m_TerrainMap[i][j][1], 16, 16, 208, 160);

			// quad generation
			Vertex2D v0({ x + m_QuadSize, y + m_QuadSize, 0.0f }, color, { uvCoordinates[2], uvCoordinates[3]}, textureSlotID);     				// right - up
			Vertex2D v1({ x + m_QuadSize, y, 0.0f }, color, { uvCoordinates[2], uvCoordinates[1] }, textureSlotID);       							// right - down
			Vertex2D v2({ x, y, 0.0f }, color, { uvCoordinates[0], uvCoordinates[1] }, textureSlotID);       										// left  - down
			Vertex2D v3({ x, y + m_QuadSize, 0.0f }, color, { uvCoordinates[0], uvCoordinates[3] }, textureSlotID);       							// left  - up
			Vertex2D quad[4] = { v0, v1, v2, v3 };
			
			// add quad to batch
			Renderer::DrawQuad(quad);
		}
	}

	// small house quads submissions
	for (uint32_t i = 0; i < 5; i++) {
		for (uint32_t j = 0; j < 4; j++) {
			// position options centered
			float x = (j + floor(m_TerrainMapWidth  / 2) - 2) * m_QuadSize;
			float y = (i + floor(m_TerrainMapHeight / 2)) * m_QuadSize;

			// texture and color options
			float textureSlotID = 1.0f;
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

			// texture atlas u/v
			glm::vec4 uvCoordinates = getUVCoordinates(m_SmallHouse[i][j][0], m_SmallHouse[i][j][1], 16, 16, 208, 160);

			// quad generation
			Vertex2D v0({ x + m_QuadSize, y + m_QuadSize, 0.0f }, color, { uvCoordinates[2], uvCoordinates[3] }, textureSlotID);     				// right - up
			Vertex2D v1({ x + m_QuadSize, y, 0.0f }, color, { uvCoordinates[2], uvCoordinates[1] }, textureSlotID);       							// right - down
			Vertex2D v2({ x, y, 0.0f }, color, { uvCoordinates[0], uvCoordinates[1] }, textureSlotID);       										// left  - down
			Vertex2D v3({ x, y + m_QuadSize, 0.0f }, color, { uvCoordinates[0], uvCoordinates[3] }, textureSlotID);       							// left  - up
			Vertex2D quad[4] = { v0, v1, v2, v3 };

			// add quad to batch
			Renderer::DrawQuad(quad);
		}
	}

	// entity rendering
	m_SpriteComponent->OnRender();

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

	return { x0, y0, x1, y1 };
}
