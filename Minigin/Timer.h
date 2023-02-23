#pragma once
#include "Singleton.h"
#include <chrono>

namespace dae
{
	class Timer : public Singleton<Timer>
	{
	public:

		void Update();

		int GetFPS();

	private:

		std::chrono::steady_clock::time_point first_tp;

		int m_FPS;

		std::chrono::time_point<std::chrono::steady_clock> m_LastTimestamp;


	};

}

