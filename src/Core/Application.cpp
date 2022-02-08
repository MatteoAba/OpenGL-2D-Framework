#pragma once

#include <stb_image.h>

#include "Application.h"

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
}

void Application::OnUpdate()
{
}

void Application::OnRender()
{
    // flush screen
    glClear(GL_COLOR_BUFFER_BIT);

    // swap buffer e gestione eventi I/O
    m_Window->ProcessEventBuffer();
}
