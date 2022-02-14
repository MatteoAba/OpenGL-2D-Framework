#pragma once

#include "../cglph.h"
#include "Log.h"

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

// creandolo in una funzione stampa il tempo trascorso dalla sua creazione alla fine di quella funzione
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