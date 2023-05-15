#pragma once
#include <string>
#include <thread>
#include <queue>
#include <mutex>

#include "Singleton.h"

namespace dae
{



	using sound_id = unsigned short;
	struct SoundData
	{
		enum class SoundType
		{
			Music,
			SoundEffect,
		};

		sound_id id{};
		float volume{ 1 };
		SoundType soundType{ SoundType::SoundEffect };
		std::string filePath{ "" };
		bool loadFile{ false };
	};

	class SoundSystem;
	class SoundManager final : public Singleton<SoundManager>
	{
	public:

		void Init(const std::string& dataPath);

		void NotifySound(SoundData soundData);

		void Quit();

	private:
		friend class Singleton<SoundManager>;

		SoundManager();

		~SoundManager();

		SoundManager(const SoundManager& other) = delete;
		SoundManager(SoundManager&& other) = delete;
		SoundManager& operator=(const SoundManager& other) = delete;
		SoundManager& operator=(SoundManager&& other) = delete;


		std::string m_dataPath;
		std::unique_ptr<SoundSystem> m_SoundSystem{ nullptr };

		std::queue<SoundData> m_EventQueue;
		std::mutex m_QueueMutex;
		std::condition_variable m_QueueCondition;
		std::atomic<bool> m_ThreadRunning{ false };

		std::jthread m_SoundThread;
		void SoundThread();

		void PlaySound(SoundData soundData);
		void LoadSound(SoundData soundData);
	};

}
