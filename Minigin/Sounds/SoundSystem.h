#pragma once
#include <string>

namespace dae
{
	using sound_id = unsigned short;

	struct SoundData
	{
		//TODO: make constructors for music and sound effect

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

	class SoundSystem
	{
	public:
		SoundSystem() = default;
		virtual ~SoundSystem() = default;


		SoundSystem(const SoundSystem& other) = delete;
		SoundSystem(SoundSystem&& other) = delete;
		SoundSystem& operator=(const SoundSystem& other) = delete;
		SoundSystem& operator=(SoundSystem&& other) = delete;

		virtual void Init(const std::string& m_DataPath) = 0;
		virtual void Quit() = 0;

		virtual void Mute() = 0;

		virtual void HandleSoundData(SoundData soundData) = 0;
	};

}
