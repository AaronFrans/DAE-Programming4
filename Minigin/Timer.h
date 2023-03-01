#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	class Timer : public Singleton<Timer>
	{
	public:

		void Update();

		float GetDeltaTime();
		float GetTimeStep();

	private:

		float m_DeltaTime{};
		int m_TargetFps{ 144 };
		float m_Timestep{ 1.f / m_TargetFps };

		std::chrono::time_point<std::chrono::steady_clock> m_LastTimestamp{ std::chrono::high_resolution_clock::now() };


	};

}

