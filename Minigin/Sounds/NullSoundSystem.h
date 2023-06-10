#pragma once
#include "SoundSystem.h"

namespace dae
{

	class NullSoundSystem final : public SoundSystem
	{

		void Init(const std::string&) override {};
		void Quit() override {};
		void Mute() override {};
		void HandleSoundData(SoundData) override {};
	};

}
