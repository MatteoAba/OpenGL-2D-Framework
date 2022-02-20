#include "../Core/Log.h"
#include "TestLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "../Core/Application.h"

void startImGui(Window* window);
void ImGuiRenderBegin(bool* show_demo_window);
void ImGuiDrawViewport(uint32_t colorAttachmentID);
void ImGuiRenderEnd(Window* window);
void ImGuiDelete();

void TestLayer::OnAttach()
{	
	// ImGui
	startImGui(m_Owner->GetWindow());
	
	// <------ TRIANGOLO ------>

	// shader per il triangolo
	m_Shader = new Shader("Quad", "assets/Shader/triangolo.vert", "assets/Shader/triangolo.frag");

	// vertices
	float vertices[] = {
		// positions            // colors
		 0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f
	};

	// vertex buffer
	m_VBO = new VertexBuffer((void*)vertices, sizeof(vertices));

	// indices
	uint32_t indices[] = {
		0, 1, 2,
		3, 0, 2
	};

	// index buffer
	m_IBO = new IndexBuffer((void*)indices, sizeof(indices));

	// vertex array
	m_VAO = new VertexArray();
	VertexLayout layout;
	layout.AddVertexGroup({ DataType::CGL_FLOAT, 3, false });
	layout.AddVertexGroup({ DataType::CGL_FLOAT, 3, false });
	m_VAO->AddBuffer(layout);

	// <------ FINE TRIANGOLO ------>

	// <------ FRAMEBUFFER ------>

	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// creo il color attachment
	glGenTextures(1, &m_TCB);
	glBindTexture(GL_TEXTURE_2D, m_TCB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Owner->GetWindow()->GetWidth(), m_Owner->GetWindow()->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TCB, 0);

	// creo il renderbuffer per il depth e stencil attachment
	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Owner->GetWindow()->GetWidth(), m_Owner->GetWindow()->GetHeight());                 // 24 bit per depth e 8 bit per stencil
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

	// controllo se il frame buffer è completo
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG_ERROR("FRAMEBUFFER INCOMPLETO");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// <------ FINE FRAMEBUFFER ------>
}

void TestLayer::OnDetach()
{
	// deallocazione shader
	delete m_Shader;

	// deallocazione vertex array
	delete m_VAO;

	// deallocazione vertex buffer
	delete m_VBO;

	// deallocazione index buffer
	delete m_IBO;

	// deallocazione framebuffer
	glDeleteFramebuffers(1, &m_FBO);

	// deallocazione ImGui
	ImGuiDelete();
}

void TestLayer::OnEvent(Event e)
{
}

void TestLayer::OnUpdate(float ts)
{
}

void TestLayer::OnRender()
{
	ImGuiRenderBegin(&m_Show_demo_window);

	// rendering del triangolo nel Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	m_Shader->Bind();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_VAO->Bind();
	m_IBO->Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	m_Shader->Unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// passo i dati del framebuffer a ImGui per il viewport
	ImGuiDrawViewport(m_TCB);
	
	ImGuiRenderEnd(m_Owner->GetWindow());
}

void startImGui(Window* window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 440 core");

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);
}

void ImGuiRenderBegin(bool* show_demo_window)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	ImGui::ShowDemoWindow(show_demo_window);
}

void ImGuiDrawViewport(uint32_t colorAttachmentID)
{

	ImGui::Begin("Viewport");
	{
		ImVec2 wsize = ImGui::GetWindowSize();
		ImGui::Image((ImTextureID)colorAttachmentID, wsize, ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();

}

void ImGuiRenderEnd(Window* window)
{
	// update display size
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)window->GetWidth(), (float)window->GetHeight());

	// Rendering
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void ImGuiDelete()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}