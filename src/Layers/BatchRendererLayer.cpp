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

    Vertex2D v0({ 0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f }, 2.0f);       // right - up
    Vertex2D v1({ 0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f }, 2.0f);       // right - down
    Vertex2D v2({-0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f }, 2.0f);       // left  - down
    Vertex2D v3({-0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f }, 2.0f);       // left  - up
    Vertex2D vertices[] = { v0, v1, v2, v3 };

	// vertex buffer
	m_VBO = new VertexBuffer((void*)vertices, sizeof(vertices));

	// indices
	uint32_t indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// index buffer
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

	// quad rendering
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3((float)m_Owner->GetWindow()->GetViewportWidth() / 2, (float)m_Owner->GetWindow()->GetViewportHeight() / 2, 0.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 1.0f));
	glm::mat4 model = translation * scale;
	m_WhiteTexture->Bind(0);
	m_BrickTexture->Bind(1);
	m_IconTexture->Bind(2);
	m_Shader->Bind();
	m_Shader->SetMat4("u_Model", model);
	m_Shader->SetMat4("u_View", m_Camera->GetView());
	m_Shader->SetMat4("u_Projection", m_Camera->GetProjection());
	m_Shader->Unbind();
	Renderer::DrawQuad(m_VAO, m_IBO, m_Shader, m_FBO);
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