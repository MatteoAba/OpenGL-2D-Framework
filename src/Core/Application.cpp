#pragma once

#include <stb_image.h>

#include "Application.h"
#include "../Layers/TestLayer.h"

Application::Application(int width, int height, const std::string& title)
    : m_LastFrame(0), m_TimeStep(0), m_Running(true)
{
    // creazione finestra
    Log::Init();
    LOG_TRACE("Creazione finestra {}x{} e contesto OpenGL", width, height);
    m_Window = new Window(this, { width, height, title });
    m_Window->SetVSync(1);

    // creazione layer stack
    m_LayerStack = new LayerStack(this);

    // DA TOGLIERE
    // glEnable(GL_DEPTH_TEST);
    stbi_set_flip_vertically_on_load(true);
}

Application::~Application()
{
    delete m_Window;
    delete m_LayerStack;
}

void Application::Inizialize()
{
    Layer* test = new TestLayer("Test Layer");
    m_LayerStack->PushLayer(test);
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
    // trasmetto gli eventi ai layer e svuoto la coda
    m_LayerStack->OnEvent(m_EventQueue);
    m_EventQueue.clear();
}

void Application::OnUpdate()
{
    m_LayerStack->OnUpdate(m_TimeStep);
}

void Application::OnRender()
{
    // flush screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_LayerStack->OnRender();

    // swap buffer e gestione eventi I/O
    m_Window->ProcessEventBuffer();
}
