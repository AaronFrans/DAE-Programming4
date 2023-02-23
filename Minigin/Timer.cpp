#include "Timer.h"



void dae::Timer::Update()
{
	m_LastTimestamp = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - m_LastTimestamp).count();
	m_FPS = 1.0f / m_DeltaTime;

}

float dae::Timer::GetFPS()
{
	return m_FPS;
}

float dae::Timer::GetDeltaTime()
{
	return m_DeltaTime;
}
