#include "Timer.h"



void dae::Timer::Update()
{
	m_DeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - m_LastTimestamp).count();
	m_FPS = 1.0f / m_DeltaTime;

	m_LastTimestamp = std::chrono::high_resolution_clock::now();
}

int dae::Timer::GetFPS()
{
	return static_cast<int>(m_FPS);
}

float dae::Timer::GetDeltaTime()
{
	return m_DeltaTime;
}
