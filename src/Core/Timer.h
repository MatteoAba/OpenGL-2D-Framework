#pragma once

#include "../oglph.h"
#include "Log.h"
#include "../Renderer/Renderer.h"

class Timer 
{
public:
	Timer() { Reset(); }
	void Reset() { m_StartingTime = std::chrono::high_resolution_clock::now(); }
	float Elapsed() { return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_StartingTime).count() * 0.001f * 0.001f * 0.001f; }
	float MillisecondsElapsed() { return Elapsed() * 1000.0f; }

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartingTime;
};

// log the time between its creation and the end of tits scope
class ScopedTimer
{
public:
	ScopedTimer(const std::string& name)
		: m_Name(name) {}
	~ScopedTimer() { LOG_WARN("[TIMER] {} : {}ms", m_Name, m_Timer.MillisecondsElapsed()); }

private:
	Timer m_Timer;
	std::string m_Name;
};

#define LOG_DURATION(...)	ScopedTimer duration(__VA_ARGS__)

// update render stats 
class StatsTimer
{
public:
	StatsTimer(const std::string& name)
		: m_Name(name) {}
	~StatsTimer() 
	{ 
		if (m_Name == "Frametime")
			Renderer::SetFrameTime(m_Timer.MillisecondsElapsed());
		if (m_Name == "Events")
			Renderer::SetEventTime(m_Timer.MillisecondsElapsed());
		else if (m_Name == "Update")
			Renderer::SetUpdateTime(m_Timer.MillisecondsElapsed());
		else if (m_Name == "Render")
			Renderer::SetRenderTime(m_Timer.MillisecondsElapsed());
		// else:
			// sistem to put in ImGui automatically 
	}

private:
	Timer m_Timer;
	std::string m_Name;
};

#define PROFILE_DURATION(...)	StatsTimer duration(__VA_ARGS__)