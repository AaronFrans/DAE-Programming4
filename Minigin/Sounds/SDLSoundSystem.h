#pragma once
#include <memory>
#include <string>
#include "Sounds/SoundSystem.h"

namespace dae
{
	class SDLSoundSystem final: public SoundSystem
	{
	public:

		//this is an empty constructor
		//used as non default for the impl
		SDLSoundSystem();

		//this is an empty destructor
		//used as non default for the impl
		~SDLSoundSystem();

		SDLSoundSystem(const SDLSoundSystem& other) = delete;
		SDLSoundSystem(SDLSoundSystem&& other) = delete;
		SDLSoundSystem& operator=(const SDLSoundSystem& other) = delete;
		SDLSoundSystem& operator=(SDLSoundSystem&& other) = delete;

		void Init() override;
		void Quit() override;

		void PlaySound(unsigned short id, SoundType soundType, float volume) override;
		void LoadSound(unsigned short id, const std::string& filepath) override;

		bool IsSoundLoaded(unsigned short id) override;


	private:
		class SDLMixerImpl;
		std::unique_ptr<SDLMixerImpl> m_pImpl{ nullptr };

	};
}

