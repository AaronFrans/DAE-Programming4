#include "Timer.h"



void dae::Timer::Update()
{
	auto now = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - m_LastTimestamp).count();
	m_LastTimestamp = now;
}


const float dae::Timer::GetDeltaTime() const
{
	return m_DeltaTime;
}

const float dae::Timer::GetTimeStep() const
{
	return m_Timestep;
}

const int dae::Timer::GetFrameTime() const
{
	return m_FrameTime;
}

const std::chrono::time_point<std::chrono::steady_clock> dae::Timer::GetLastTimeStamp() const
{
	return m_LastTimestamp;
}
