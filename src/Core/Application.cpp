#pragma once

#include "Application.h"
#include "Timer.h"
#include "../Layers/TestLayer.h"

Application::Application(int width, int height, const std::string& title)
    : m_LastFrame(0), m_TimeStep(0), m_Running(true)
{
    // window creation
    Log::Init();
    LOG_TRACE("Creazione finestra {}x{} e contesto OpenGL", width, height);
    m_Window = new Window(this, { width, height, title });
    m_Window->SetVSync(1);

    // layer stack creation
    m_LayerStack = new LayerStack(this);

    // TODO: move to renderer
    // glEnable(GL_DEPTH_TEST);
    // blending mode
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Application::~Application()
{
    delete m_Window;
    delete m_LayerStack;
}

void Application::Inizialize()
{
    LOG_DURATION("Application Startup");

    Layer* test = new TestLayer("Test Layer", this);
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
    // dispatch event to layers
    m_LayerStack->OnEvent(m_EventQueue);
    m_EventQueue.clear();
}

void Application::OnUpdate()
{
    m_LayerStack->OnUpdate(m_TimeStep);
}

void Application::OnRender()
{
    // flush screen     TODO: move to renderer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_LayerStack->OnRender();

    // swap buffer and I/O events handing
    m_Window->ProcessEventBuffer();
}
