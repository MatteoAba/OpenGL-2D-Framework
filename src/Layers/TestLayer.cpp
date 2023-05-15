#include "TestLayer.h"

#include "../cglph.h"
#include "../Core/Log.h"
#include "../Core/Application.h"
#include "../Renderer/Renderer.h"

TestLayer::TestLayer(std::string name, Application* owner)
	: Layer(name, owner)
{
}

void TestLayer::OnAttach()
{	
	// camera e controller
	float width = (float)(m_Owner->GetWindow()->GetViewportWidth());
	float height = (float)(m_Owner->GetWindow()->GetViewportHeight());
	m_Camera = new OrthographicCamera(width, height);
	m_CameraController = new OrthographicCameraController(m_Camera);

	// text
	m_Text = new Text("assets/Font/arial.ttf", m_Owner);
	m_TextPosition = glm::vec2(48.0f, 48.0f * 2);
	m_TextScale = 0.6f;
	
	// <-------- QUAD --------->

	// shader per il triangolo
	m_Shader = new Shader("Quad", "assets/Shader/Quads.vert", "assets/Shader/Quads.frag");

	// vertices
	float vertices[] = {
		// positions            // colors			// coordinate texture
		200.0f, 200.0f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,		// right - up
		200.0f, 100.0f, 0.0f,   1.0f, 0.0f, 0.0f,	1.0f, 0.0f,		// right - down	
		100.0f, 100.0f, 0.0f,   0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// left  - down
		100.0f, 200.0f, 0.0f,   0.0f, 0.0f, 1.0f,	0.0f, 1.0f		// left  - up
	};

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
	layout.AddVertexGroup({ DataType::CGL_FLOAT, 3, false });
	layout.AddVertexGroup({ DataType::CGL_FLOAT, 2, false });
	m_VAO->AddBuffer(layout, *m_VBO);

	// texture
	m_Texture = new Texture("assets/Img/bricks.png");

	// framebuffer
	m_FBO = new Framebuffer(m_Owner, true);

	// <------ FINE QUAD ------>
}

void TestLayer::OnDetach()
{
	// deallocazione text
	delete m_Text;

	// deallocazione shader
	delete m_Shader;

	// deallocazione vertex array
	delete m_VAO;

	// deallocazione vertex buffer
	delete m_VBO;

	// deallocazione index buffer
	delete m_IBO;

	// deallocazione Texture
	delete m_Texture;

	// deallocazione framebuffer
	delete m_FBO;

	// deallocazione camera e controller
	delete m_Camera;
	delete m_CameraController;
}

void TestLayer::OnEvent(Event e)
{
	m_CameraController->OnEvent(e);
}

void TestLayer::OnUpdate(float ts)
{
	m_CameraController->OnUpdate(ts);
}

void TestLayer::OnRender()
{
	// clear the framebuffer
	m_FBO->Bind();
	Renderer::ClearScreen();
	m_FBO->Unbind();

	// quad rendering
	m_Texture->Bind(0);
	m_Shader->Bind();
	m_Shader->SetInt("texture1", 0);
	m_Shader->SetMat4("model", glm::mat4(1.0f));
	m_Shader->SetMat4("view", m_Camera->GetView());
	m_Shader->SetMat4("projection", m_Camera->GetProjection());
	m_Shader->Unbind();
	Renderer::DrawQuad(m_VAO, m_IBO, m_Shader, m_FBO);

	// text rendering
	m_Text->RenderText("Sono un fallito", m_TextPosition, m_TextScale, glm::vec3(1.0f, 0.0f, 0.0f), m_FBO);
	m_Text->RenderText("Deh", m_TextPosition + glm::vec2(0.0f, 48.0f), m_TextScale, glm::vec3(1.0f, 1.0f, 0.0f), m_FBO);
}

void TestLayer::OnImGuiRender()
{
	ImGui::Begin("Viewport");
	{
		ImVec2 wsize = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)(uint64_t)m_FBO->GetColorAttachment(), wsize, ImVec2(0, 1), ImVec2(1, 0));

		// update viewport
		m_Owner->GetWindow()->UpdateViewport((uint32_t)wsize.x, (uint32_t)wsize.y);
	}
	ImGui::End();

	// controller for speed
	ImGui::Begin("Settings");
	{
		ImGui::SliderFloat("Camera Speed", m_CameraController->GetSpeedPointer(), 0.0f, 300.0f);
	}
	ImGui::End();
}