#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	class Timer : public Singleton<Timer>
	{
	public:

		void Update();

		float GetDeltaTime() const;
		float GetTimeStep() const;
		int GetFrameTime() const;
		std::chrono::time_point<std::chrono::steady_clock> GetLastTimeStamp() const;

	private:

		float m_DeltaTime{};
		const int m_TargetFps{  144 };
		const float m_Timestep{ 0.02f };
		int m_FrameTime{ 1000 / m_TargetFps };

		std::chrono::time_point<std::chrono::steady_clock> m_LastTimestamp{ std::chrono::high_resolution_clock::now() };


	};

}

