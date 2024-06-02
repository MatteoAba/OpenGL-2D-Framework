#include "BatchRendererLayer.h"

#include "../oglph.h"
#include "../Core/Log.h"
#include "../Core/Application.h"
#include "../Renderer/Renderer.h"

BatchRendererLayer::BatchRendererLayer(std::string name, Application* owner)
	: Layer(name, owner), m_CheckerboardRows(8), m_CheckerboardColumns(8), m_QuadSize(70.0f)
{
}

void BatchRendererLayer::OnAttach()
{	
	// camera e controller
	float width = (float)(m_Owner->GetWindow()->GetViewportWidth());
	float height = (float)(m_Owner->GetWindow()->GetViewportHeight());
	m_Camera = new OrthographicCamera(width, height);
	m_CameraController = new OrthographicCameraController(m_Camera);

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
	glm::u32vec3 textureArraySize = { 1024U, 1024U, 3U };			// { width, height, layers number }
	Renderer::SetupBatchRendering(maxVerticesCount, m_Shader, textureArraySize, m_FBO);
	m_TextureInfo.push_back(Renderer::AddTextureToBatch("assets/Img/bricks.png"));
	m_TextureInfo.push_back(Renderer::AddTextureToBatch("assets/Img/icon.png"));
	Renderer::SetBlending(true);
}

void BatchRendererLayer::OnDetach()
{
	// stop batch renderer
	Renderer::StopBatchRendering();

	delete m_Shader;
	delete m_FBO;
	delete m_Camera;
	delete m_CameraController;
}

void BatchRendererLayer::OnEvent(Event e)
{
	m_CameraController->OnEvent(e);
}

void BatchRendererLayer::OnUpdate(float ts)
{
	m_CameraController->OnUpdate(ts);
}

void BatchRendererLayer::OnRender()
{
	// clear the framebuffer
	m_FBO->Bind();
	Renderer::ClearScreen();
	m_FBO->Unbind();

	// quad submission
	Renderer::InitNewBatch();
	for (uint32_t i = 0; i < m_CheckerboardRows; ++i) {
		for (uint32_t j = 0; j < m_CheckerboardColumns; ++j) {
			// position options
			float x = j * m_QuadSize;
			float y = i * m_QuadSize; 
			
			// texture and color options
			float textureSlotID = (i + j) % 2 == 0 ? m_TextureInfo[0][2] : m_TextureInfo[1][2];
			float textureWidth  = (i + j) % 2 == 0 ? m_TextureInfo[0][0] : m_TextureInfo[1][0];
			float textureHeight = (i + j) % 2 == 0 ? m_TextureInfo[0][1] : m_TextureInfo[1][1];
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

			// quad generation
			Vertex2D v0({ x + m_QuadSize, y + m_QuadSize, 0.0f }, color, { textureWidth, textureHeight }, textureSlotID);     	// right - up
			Vertex2D v1({ x + m_QuadSize, y, 0.0f }, color, { textureWidth, 0.0f }, textureSlotID);       						// right - down
			Vertex2D v2({ x, y, 0.0f }, color, { 0.0f, 0.0f }, textureSlotID);       											// left  - down
			Vertex2D v3({ x, y + m_QuadSize, 0.0f }, color, { 0.0f, textureHeight }, textureSlotID);       						// left  - up
			Vertex2D quad[4] = { v0, v1, v2, v3 };

			// add quad to batch
			Renderer::DrawQuad(quad);
		}
	}

	// quads rendering
	m_Shader->Bind();
	m_Shader->SetMat4("u_View", m_Camera->GetView());
	m_Shader->SetMat4("u_Projection", m_Camera->GetProjection());
	m_Shader->Unbind();
	Renderer::EndBatch();
}

void BatchRendererLayer::OnImGuiRender()
{
	ImGui::Begin("Viewport");
	{
		ImVec2 wsize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)(uint64_t)m_FBO->GetColorAttachment(), wsize, ImVec2(0, 1), ImVec2(1, 0));

		// update viewport
		m_Owner->GetWindow()->UpdateViewport((uint32_t)wsize.x, (uint32_t)wsize.y);
	}
	ImGui::End();

	// settings for camera and rendered checkerboard
	ImGui::Begin("Settings");
	{
		ImGui::SliderFloat("Quad Size", &m_QuadSize, 25.0f, 100.0f);
		ImGui::SliderFloat("Camera Speed", m_CameraController->GetSpeedPointer(), 300.0f, 600.0f);
	}
	ImGui::End();
}