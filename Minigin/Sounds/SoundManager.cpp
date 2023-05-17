#include "SoundManager.h"

#include <iostream>
#include <thread>


#include "SDLSoundSystem.h"
#include "NullSoundSystem.h"

void dae::SoundManager::Init(const std::string& dataPath)
{
	m_dataPath = dataPath;
	
	m_SoundSystem->Init();

	m_ThreadRunning = true;
	m_SoundThread = std::jthread(&SoundManager::SoundThread, this);
}

void dae::SoundManager::NotifySound(SoundData soundData)
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	soundData.filePath = m_dataPath + soundData.filePath;
	m_EventQueue.push(soundData);

	m_QueueCondition.notify_all();
}

void dae::SoundManager::SetSoundSystem(std::unique_ptr<SoundSystem> soundSystem)
{
	m_SoundSystem->Quit();

	m_SoundSystem = std::move(soundSystem);
}

void dae::SoundManager::Quit()
{
	m_ThreadRunning = false;

	m_QueueCondition.notify_all();

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

void dae::SoundManager::PlaySound(SoundData soundData)
{
	if (!m_SoundSystem->IsSoundLoaded(soundData.id))
	{
		m_SoundSystem->LoadSound(soundData.id, soundData.filePath);
	}

	m_SoundSystem->PlaySound(soundData.id, static_cast<SDLSoundSystem::SoundType>(soundData.soundType), soundData.volume);
}

void dae::SoundManager::LoadSound(SoundData soundData)
{
	if (!m_SoundSystem->IsSoundLoaded(soundData.id))
	{
		m_SoundSystem->LoadSound(soundData.id, soundData.filePath);
	}
}


void dae::SoundManager::SoundThread()
{
	while (m_ThreadRunning)
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_QueueCondition.wait(lock, [&] {

			if (!m_ThreadRunning)
				return true;

			return !m_EventQueue.empty();
			});


		while (!m_EventQueue.empty())
		{
			SoundData data = m_EventQueue.front();
			m_EventQueue.pop();

			if (data.loadFile)
				LoadSound(data);
			else
				PlaySound(data);
		}

		lock.unlock();
	}
}