#include "ImGuiLayer.h"

#include "../Core/Application.h"
#include "../Renderer/Renderer.h"

ImGuiLayer::ImGuiLayer(std::string name, Application* owner)
	: Layer(name, owner), m_OpenGLVersion("#version 440 core"), m_ShowDemo(true)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;      	// Enable Multi-Viewport / Platform Windows
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
	ImGui_ImplGlfw_InitForOpenGL(m_Owner->GetWindow()->GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init(m_OpenGLVersion.c_str());

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

ImGuiLayer::~ImGuiLayer()
{
	// cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::Begin()
{
	// start ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// main window
	#ifdef IMGUI_HAS_VIEWPORT
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	#else 
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	#endif
	// ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::Begin("Main Window", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

	// demo windows
	// ImGui::ShowDemoWindow(&m_ShowDemo);
}

void ImGuiLayer::End()
{
	// TODO: Layer->OnImGuiRender() here???

	// utils for render options
	bool vsync = Renderer::GetVSync();
	const char* labels[] = { "Unlocked", "30", "60", "144", "240", "360" };
	int selectedItems = 0;
	if (Renderer::GetMaxframetime() == 1.0f / 30.0f)
		selectedItems = 1;
	else if (Renderer::GetMaxframetime() == 1.0f / 60.0f)
		selectedItems = 2;
	else if (Renderer::GetMaxframetime() == 1.0f / 144.0f)
		selectedItems = 3;
	else if (Renderer::GetMaxframetime() == 1.0f / 240.0f)
		selectedItems = 4;
	else if (Renderer::GetMaxframetime() == 1.0f / 360.0f)
		selectedItems = 5;
	int oldSelected = selectedItems;
	bool msaa = Renderer::GetMSAA();

	// render options
 	ImGui::Begin("Render Option");
	{
		// V-Sync
		ImGui::Checkbox("V-Sync Enabled", &vsync);
		if (vsync != Renderer::GetVSync())
			Renderer::SetVSync(vsync);

		// Framerate Limit
		ImGui::Combo("Framerate Limit", &selectedItems, labels, IM_ARRAYSIZE(labels));
		if (selectedItems != oldSelected) {
			selectedItems = selectedItems ? atoi(labels[selectedItems]) : 0;
			Renderer::SetMaxFramerate(selectedItems);
		}

		// V-Sync
		ImGui::Checkbox("MSAAx4 Enabled", &msaa);
		if (msaa != Renderer::GetMSAA())
			Renderer::SetMSAA(msaa);
	}
	ImGui::End();

	// render statistics
	RendererStats stats(Renderer::GetStats());
	ImGui::Begin("Render Statistics");
	{
		// general stats
		ImGui::Text("Frametime: %.3fms", stats.frameTime * 1000);
		ImGui::Text("Draw calls: %d", stats.drawCall);
		ImGui::Text("Number of quads: %d", stats.renderedQuad);
		ImGui::Text("Framerate: %.0ffps", 1.0f / stats.frameTime);
		
		ImGui::Separator();

		// specific frame stats
		ImGui::Text("Event  Time: %.3fms", stats.eventTime);
		ImGui::Text("Update Time: %.3fms", stats.updateTime);
		ImGui::Text("Render Time: %.3fms", stats.renderTime);
	}
	ImGui::End();
	Renderer::SetDrawCall(0);
	Renderer::SetRenderedQuad(0);
	
	// update display size
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)m_Owner->GetWindow()->GetWidth(), (float)m_Owner->GetWindow()->GetHeight());

	// close main window
	ImGui::End();

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
