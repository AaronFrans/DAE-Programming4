#include "Timer.h"



void dae::Timer::Update()
{
	auto now = std::chrono::high_resolution_clock::now();
	m_DeltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - m_LastTimestamp).count();
	m_LastTimestamp = now;
	//sleep -> expected time - time for update + rendetr
}


float dae::Timer::GetDeltaTime()
{
	return m_DeltaTime;
}

float dae::Timer::GetTimeStep()
{
	return m_Timestep;
}
