#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	class Timer : public Singleton<Timer>
	{
	public:

		void Update();

		float GetFPS();
		float GetDeltaTime();

	private:

		std::chrono::steady_clock::time_point first_tp;

		float m_FPS{};
		float m_DeltaTime{};

		std::chrono::time_point<std::chrono::steady_clock> m_LastTimestamp;


	};

}

