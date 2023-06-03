#pragma once
#include <string>
#include <memory>

#include "Singleton.h"

namespace dae
{

	class SoundSystem;
	class SoundManager final : public Singleton<SoundManager>
	{
	public:

		void Init(const std::string& dataPath);

		void SetSoundSystem(std::unique_ptr<SoundSystem> soundSystem);

		SoundSystem* GetSoundSystem() { return m_SoundSystem.get(); };

		void Quit();

	private:
		friend class Singleton<SoundManager>;
		std::string m_DataPath;

		std::unique_ptr<SoundSystem> m_SoundSystem{ nullptr };
		SoundManager();

		~SoundManager();

		SoundManager(const SoundManager& other) = delete;
		SoundManager(SoundManager&& other) = delete;
		SoundManager& operator=(const SoundManager& other) = delete;
		SoundManager& operator=(SoundManager&& other) = delete;
	};

}
