#pragma once
#include "SoundSystem.h"

namespace dae
{

	class NullSoundSystem final : public SoundSystem
	{

		void Init(const std::string&) override {};
		virtual void Quit() override {};

		virtual void HandleSoundData(SoundData) {};
	};

}
