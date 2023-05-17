#pragma once
#include <string>

namespace dae
{

	class SoundSystem
	{
	public:
		enum class SoundType
		{
			Music,
			SoundEffect,
		};

		SoundSystem() = default;
		virtual ~SoundSystem() = default;


		SoundSystem(const SoundSystem& other) = delete;
		SoundSystem(SoundSystem&& other) = delete;
		SoundSystem& operator=(const SoundSystem& other) = delete;
		SoundSystem& operator=(SoundSystem&& other) = delete;

		virtual void Init() = 0;
		virtual void Quit() = 0;

		virtual void PlaySound(unsigned short id, SoundType soundType, float volume) = 0;
		virtual void LoadSound(unsigned short id, const std::string& filepath) = 0;

		virtual bool IsSoundLoaded(unsigned short id) = 0;

	};

}
