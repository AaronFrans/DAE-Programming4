#pragma once
#include "Sounds/SoundSystem.h"

namespace dae
{

	class NullSoundSystem final : public SoundSystem
	{

		void Init() override {};
		virtual void Quit() override {};

		void PlaySound(unsigned short, SoundType, float) override {};
		void LoadSound(unsigned short, const std::string&) override {};

		bool IsSoundLoaded(unsigned short) override { return false; };
	};

}
