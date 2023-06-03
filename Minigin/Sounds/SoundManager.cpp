#include "SoundManager.h"

#include <iostream>
#include <thread>


#include "SDLSoundSystem.h"
#include "NullSoundSystem.h"

void dae::SoundManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	m_SoundSystem->Init(dataPath);
}
void dae::SoundManager::SetSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
{
	m_SoundSystem->Quit();

	m_SoundSystem = std::move(soundSystem);

	m_SoundSystem->Init(m_DataPath);
}

void dae::SoundManager::Quit()
{

	m_SoundSystem->Quit();

	if (m_SoundSystem)
	{
		m_SoundSystem = nullptr;
	}

}

dae::SoundManager::SoundManager()
{
	m_SoundSystem = std::make_unique<NullSoundSystem>();
}

dae::SoundManager::~SoundManager()
{
}
