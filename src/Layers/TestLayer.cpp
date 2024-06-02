#include "TestLayer.h"

#include "../oglph.h"
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
		// positions			// colors (RGB)			// coordinate texture
		 0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f,		// right - up
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,		// right - down	
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f,		// left  - down
		-0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f		// left  - up
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
	layout.AddVertexGroup({ DataType::OGL_FLOAT, 3, false });
	layout.AddVertexGroup({ DataType::OGL_FLOAT, 3, false });
	layout.AddVertexGroup({ DataType::OGL_FLOAT, 2, false });
	m_VAO->AddBuffer(layout, *m_VBO);

	// textures
	m_Texture = new Texture("assets/Img/bricks.png");
	m_TextureAtlas = new Texture("assets/Img/terrain_atlas.png", 32, 32);

	// framebuffer
	m_FBO = new Framebuffer(m_Owner, true);

	// <------ END QUAD ------>

	// <----- ATLAS QUAD ----->

	m_AtlasRow = 0;
	m_AtlasColumn = 0;

	// buffers
	m_AtlasVBO = new VertexBuffer(8 * 4 * sizeof(float));
	m_AtlasVAO = new VertexArray();
	m_AtlasVAO->AddBuffer(layout, *m_AtlasVBO);

	// <------ END QUAD ------>
}

void TestLayer::OnDetach()
{
	delete m_Text;
	delete m_Shader;
	delete m_VAO;
	delete m_AtlasVAO;
	delete m_VBO;
	delete m_AtlasVBO;
	delete m_IBO;
	delete m_Texture;
	delete m_TextureAtlas;
	delete m_FBO;
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
	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3((float)m_Owner->GetWindow()->GetViewportWidth() / 3, (float)m_Owner->GetWindow()->GetViewportHeight() / 2, 0.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 1.0f));
	glm::mat4 model = translation * scale;
	m_Texture->Bind(0);
	m_Shader->Bind();
	m_Shader->SetInt("texture1", 0);
	m_Shader->SetMat4("u_Model", model);
	m_Shader->SetMat4("u_View", m_Camera->GetView());
	m_Shader->SetMat4("u_Projection", m_Camera->GetProjection());
	m_Shader->Unbind();
	Renderer::DrawQuad(m_VAO, m_IBO, m_Shader, m_FBO);

	// atlas quad vertices
	glm::vec4 textureCoords = m_TextureAtlas->GetSubTextureCoordinates(m_AtlasRow, m_AtlasColumn);
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		textureCoords[2], textureCoords[3],
		 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		textureCoords[2], textureCoords[1],	
		-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		textureCoords[0], textureCoords[1],
		-0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		textureCoords[0], textureCoords[3]
	};

	// atlas quad rendering
	m_AtlasVBO->SubmitData(vertices, sizeof(vertices));
	translation = glm::translate(glm::mat4(1.0f), glm::vec3((float)m_Owner->GetWindow()->GetViewportWidth() * 2 / 3, (float)m_Owner->GetWindow()->GetViewportHeight() / 2, 0.0f));
	scale = glm::scale(glm::mat4(1.0f), glm::vec3(32.0f, 32.0f, 1.0f));
	model = translation * scale;
	m_TextureAtlas->Bind(0);
	m_Shader->Bind();
	m_Shader->SetInt("texture1", 0);
	m_Shader->SetMat4("u_Model", model);
	m_Shader->SetMat4("u_View", m_Camera->GetView());
	m_Shader->SetMat4("u_Projection", m_Camera->GetProjection());
	m_Shader->Unbind();
	Renderer::DrawQuad(m_AtlasVAO, m_IBO, m_Shader, m_FBO);

	// text rendering
	m_Text->RenderText("Text Red", m_TextPosition, m_TextScale, glm::vec3(1.0f, 0.0f, 0.0f), m_FBO);
	m_Text->RenderText("Text Yellow", m_TextPosition + glm::vec2(0.0f, 48.0f), m_TextScale, glm::vec3(1.0f, 1.0f, 0.0f), m_FBO);
}

void TestLayer::OnImGuiRender()
{
	ImGui::Begin("Viewport");
	{
		ImVec2 wsize = ImGui::GetContentRegionAvail();
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

	// controller for atlas
	ImGui::Begin("Texture Selection");
	{
		ImGui::SliderInt("Texture Row", (int*)&m_AtlasRow, 0, 31);
		ImGui::SliderInt("Texture Column", (int*)&m_AtlasColumn, 0, 31);
	}
	ImGui::End();
}