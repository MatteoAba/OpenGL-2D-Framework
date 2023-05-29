#pragma once

#include "Application.h"
#include "Timer.h"
#include "../Layers/TestLayer.h"
#include "../Layers/BatchRendererLayer.h"
#include "../Layers/TilesMapLayer.h"
#include "../Renderer/Renderer.h"

Application::Application(uint32_t width, uint32_t height, const std::string& title)
    : m_LastFrame(0), m_TimeStep(0), m_Running(true)
{
    // window creation
    Log::Init();
    LOG_TRACE("Started creation of Window {}x{} and OpenGL context", width, height);
    m_Window = new Window(this, { width, height, title });

    // layer stack creation
    m_LayerStack = new LayerStack(this);

    // renderer inizialization
    Renderer::Init(this);
    Renderer::SetVSync(true);
    Renderer::SetMSAA(false);
}

Application::~Application()
{
    delete m_Window;
    delete m_LayerStack;
}

void Application::Inizialize()
{
    LOG_DURATION("Application Startup");

    // layers setup
    // Layer* test = new TestLayer("Test Layer", this);
    // Layer* batch = new BatchRendererLayer("Batch Renderer Layer", this);
    Layer* tilesMap = new TilesMapLayer("Tiles Map Layer", this);
    m_LayerStack->PushLayer(tilesMap);
}

void Application::Run()
{
    while (m_Running)
    {
        // update timestep
        float currentTime = m_Window->GetFrameTime();
        if (currentTime - m_LastFrame < Renderer::GetMaxframetime())    // lock to max frametime
            continue;
        m_TimeStep = currentTime - m_LastFrame;
        m_LastFrame = currentTime;
        Renderer::SetFrameTime(m_TimeStep);

        // main loop function
        OnEvent();
        OnUpdate();
        OnRender();
    }
}

void Application::OnEvent()
{
    PROFILE_DURATION("Events");

    // dispatch event to layers
    m_LayerStack->OnEvent(m_EventQueue);
    m_EventQueue.clear();
}

void Application::OnUpdate()
{
    PROFILE_DURATION("Update");

    m_LayerStack->OnUpdate(m_TimeStep);
}

void Application::OnRender()
{
    PROFILE_DURATION("Render");

    // flush screen
    Renderer::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    Renderer::ClearScreen();

    // layers rendering
    m_LayerStack->OnRender();

    // swap buffer and I/O events handing
    m_Window->ProcessEventBuffer();
}
