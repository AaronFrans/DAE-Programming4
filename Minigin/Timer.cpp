#include "Timer.h"

void dae::Timer::Update()
{
		m_LastTimestamp = std::chrono::high_resolution_clock::now();
		m_FPS = 1.0f / std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - m_LastTimestamp).count();

}

int dae::Timer::GetFPS()
{
	return m_FPS;
}
