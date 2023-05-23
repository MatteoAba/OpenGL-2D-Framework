#include "BatchRendererLayer.h"

#include "../cglph.h"
#include "../Core/Log.h"
#include "../Core/Application.h"
#include "../Renderer/Renderer.h"

BatchRendererLayer::BatchRendererLayer(std::string name, Application* owner)
	: Layer(name, owner)
{
}

void BatchRendererLayer::OnAttach()
{	
	// camera e controller
	float width = (float)(m_Owner->GetWindow()->GetViewportWidth());
	float height = (float)(m_Owner->GetWindow()->GetViewportHeight());
	m_Camera = new OrthographicCamera(width, height);
	
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
    m_WhiteTexture = new Texture();
	m_BrickTexture = new Texture("assets/Img/bricks.png");
	m_IconTexture  = new Texture("assets/Img/icon.png");

    // shader
	m_Shader = new Shader("Quad", "assets/Shader/Vertex2D.vert", "assets/Shader/Vertex2D.frag");
    int samplers[32];
    for	(int i = 0; i < 32; ++i)
        samplers[i] = i;
    m_Shader->Bind();
    m_Shader->SetIntArray("u_Textures", samplers, 32);
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
	delete m_BrickTexture;
	delete m_IconTexture;
	delete m_WhiteTexture;
	delete m_FBO;
	delete m_Camera;
}

void BatchRendererLayer::OnEvent(Event e)
{
    if (e.GetType() == EventType::VIEWPORTRESIZE)
		m_Camera->ChangeProjection((float)e.GetStructure().Resize.width, (float)e.GetStructure().Resize.height);
}

void BatchRendererLayer::OnUpdate(float ts)
{
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
	for (uint32_t i = 0; i < 8; ++i) {
		for (uint32_t j = 0; j < 8; ++j) {
			float scale = 100.0f;
			float x = i * scale;
			float y = j * scale; 
			float textureSlotID = (i + j) % 2 == 0 ? 1.0f : 2.0f;
			/*float textureSlotID = 0.0f;
			glm::vec4 color; 
			if ((i + j) % 2 == 0)
				color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			else 
				color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);*/
			glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			Vertex2D v0({ x + scale, y + scale, 0.0f }, color, { 1.0f, 1.0f }, textureSlotID);       	// right - up
			Vertex2D v1({ x + scale, y, 0.0f }, color, { 1.0f, 0.0f }, textureSlotID);       			// right - down
			Vertex2D v2({ x, y, 0.0f }, color, { 0.0f, 0.0f }, textureSlotID);       					// left  - down
			Vertex2D v3({ x, y + scale, 0.0f }, color, { 0.0f, 1.0f }, textureSlotID);       			// left  - up
			std::array<Vertex2D, 4> quad = {v0, v1, v2, v3};
			memcpy(next, quad.data(), quad.size() * sizeof(Vertex2D));
			next += 4;
		}
	}
	m_VBO->SubmitData(quads, sizeof(quads));

	// quads rendering
	m_WhiteTexture->Bind(0);
	m_BrickTexture->Bind(1);
	m_IconTexture->Bind(2);
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
}