#include "MuteCommand.h"

#include "Sounds/SoundManager.h"
#include "Sounds/SoundSystem.h"

void dae::MuteCommand::Execute()
{
	SoundManager::GetInstance().GetSoundSystem()->Mute();
}
