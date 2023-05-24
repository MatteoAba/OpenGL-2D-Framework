#include "BatchRendererLayer.h"

#include "../cglph.h"
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
	
	// <-------- QUAD --------->

	// vertex buffer
	m_VBO = new VertexBuffer(1000 * 4 * sizeof(Vertex2D));

	// index buffer
	uint32_t indices[1000 * 6];
	for (int i = 0, offset = 0; i < 1000 * 6; i += 6, offset += 4) {
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;
		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;
	}
	m_IBO = new IndexBuffer((void*)indices, sizeof(indices));

	// vertex array
	m_VAO = new VertexArray();
	VertexLayout layout;
	layout.AddVertexGroup({ DataType::CGL_FLOAT, 3, false });
	layout.AddVertexGroup({ DataType::CGL_FLOAT, 4, false });
	layout.AddVertexGroup({ DataType::CGL_FLOAT, 2, false });
	layout.AddVertexGroup({ DataType::CGL_FLOAT, 1, false });
	m_VAO->AddBuffer(layout, *m_VBO);

	// textures
	m_Textures = new TextureArray(1024, 1024, 3);
	m_Textures->Bind();
	m_Textures->SubmitTexture();							// white texture
	m_Textures->SubmitTexture("assets/Img/bricks.png");
	m_Textures->SubmitTexture("assets/Img/icon.png");

    // shader
	m_Shader = new Shader("Quad", "assets/Shader/Vertex2D.vert", "assets/Shader/Vertex2D.frag");
    m_Shader->Bind();
	m_Shader->SetInt("u_Textures", 0);
	m_Shader->SetMat4("u_Model", glm::mat4(1.0f));
	m_Shader->Unbind();

	// framebuffer
	m_FBO = new Framebuffer(m_Owner, true);

	// <------ END QUAD ------>
}

void BatchRendererLayer::OnDetach()
{
	delete m_Shader;
	delete m_VAO;
	delete m_VBO;
	delete m_IBO;
	delete m_Textures;
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
	Vertex2D quads[4 * 8 * 8];
	Vertex2D* next = quads;
	for (uint32_t i = 0; i < m_CheckerboardRows; ++i) {
		for (uint32_t j = 0; j < m_CheckerboardColumns; ++j) {
			// position options
			float x = i * m_QuadSize;
			float y = j * m_QuadSize; 
			
			// texture and color options
			float textureSlotID = (i + j) % 2 == 0 ? 1.0f : 2.0f;
			float textureWidth  = (i + j) % 2 == 0 ? 1.0f : (533.0f / 1024.0f);
			float textureHeight = (i + j) % 2 == 0 ? 1.0f : (533.0f / 1024.0f);
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

			// quad generation
			Vertex2D v0({ x + m_QuadSize, y + m_QuadSize, 0.0f }, color, { textureWidth, textureHeight }, textureSlotID);     	// right - up
			Vertex2D v1({ x + m_QuadSize, y, 0.0f }, color, { textureWidth, 0.0f }, textureSlotID);       						// right - down
			Vertex2D v2({ x, y, 0.0f }, color, { 0.0f, 0.0f }, textureSlotID);       											// left  - down
			Vertex2D v3({ x, y + m_QuadSize, 0.0f }, color, { 0.0f, textureHeight }, textureSlotID);       						// left  - up
			std::array<Vertex2D, 4> quad = { v0, v1, v2, v3 };

			memcpy(next, quad.data(), quad.size() * sizeof(Vertex2D));
			next += 4;
		}
	}
	m_VBO->SubmitData(quads, sizeof(quads));

	// quads rendering
	m_Shader->Bind();
	m_Shader->SetMat4("u_View", m_Camera->GetView());
	m_Shader->SetMat4("u_Projection", m_Camera->GetProjection());
	m_Shader->Unbind();
	Renderer::DrawQuad(m_VAO, m_IBO, m_Shader, m_FBO, 6 * 8 * 8);
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