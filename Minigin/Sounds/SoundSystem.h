#pragma once
#include <memory>
#include <string>

namespace dae
{
	class SoundSystem final
	{
	public:
		
		enum class SoundType
		{
			Music,
			SoundEffect,
		};

		//this is an empty constructor
		//used as non default for the impl
		SoundSystem();

		//this is an empty destructor
		//used as non default for the impl
		~SoundSystem();

		SoundSystem(const SoundSystem& other) = delete;
		SoundSystem(SoundSystem&& other) = delete;
		SoundSystem& operator=(const SoundSystem& other) = delete;
		SoundSystem& operator=(SoundSystem&& other) = delete;

		void Init();
		void Quit();

		void PlaySound(unsigned short id, SoundType soundType, float volume);
		void LoadSound(unsigned short id, const std::string& filepath);

		bool IsSoundLoaded(unsigned short id);


	private:
		class SDLMixerImpl;
		std::unique_ptr<SDLMixerImpl> m_pImpl{ nullptr };

	};
}

