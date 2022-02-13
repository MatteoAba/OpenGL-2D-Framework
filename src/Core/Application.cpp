#pragma once

#include <stb_image.h>

#include "Application.h"
#include "../Renderer/Shader.h"

Application::Application(int width, int height, const std::string& title)
    : m_LastFrame(0), m_TimeStep(0), m_Running(true)
{
    Log::Init();
    LOG_TRACE("Creazione finestra {}x{} e contesto OpenGL", width, height);
    m_Window = new Window(this, { width, height, title });
    m_Window->SetVSync(1);
    
    glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(true);
}

Application::~Application()
{
    delete m_Window;
}

void Application::Inizialize()
{
    // creo un layer di prova
    Shader test("Triangolo", "assets/Shader/triangolo.vert", "assets/Shader/triangolo.frag");
    
}

void Application::Run()
{
    while (m_Running)
    {
        // update timestep
        float currentTime = m_Window->GetFrameTime();
        m_TimeStep = currentTime - m_LastFrame;
        m_LastFrame = currentTime;

        // main loop function
        OnEvent();
        OnUpdate();
        OnRender();
    }
}

void Application::OnEvent()
{
    m_LayerStack.OnEvent();
}

void Application::OnUpdate()
{
    m_LayerStack.OnUpdate(m_TimeStep);
}

void Application::OnRender()
{
    // flush screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_LayerStack.OnRender();

    // swap buffer e gestione eventi I/O
    m_Window->ProcessEventBuffer();
}
