#include "SoundManager.h"

#include <iostream>
#include <thread>

#include "SoundSystem.h"

//void PlayeTestSound()
//{


	//if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
	//	std::cout << "Failed To Open Audio";
	//	return;
	//}
	//
	//Mix_Chunk* soundEffect = Mix_LoadWAV("../Data/Sounds/Funny-03.wav");
	//if (soundEffect == nullptr) {
	//	std::cout << "Failed To Open Sound File";
	//	return;
	//}
	//
	//auto channelNr = Mix_PlayChannel(-1, soundEffect, 0);
	//
	//while (Mix_Playing(channelNr) != 0)
	//{
	//
	//}
	//
	//Mix_FreeChunk(soundEffect);
	//
	//Mix_CloseAudio();
	//SDL_Quit();
//}

void dae::SoundManager::Init(const std::string& dataPath)
{

	m_dataPath = dataPath;
	if (!m_SoundSystem)
		m_SoundSystem = std::make_unique<SoundSystem>();

	m_SoundSystem->Init();

	m_ThreadRunning = true;
	m_SoundThread = std::jthread(&SoundManager::SoundThread, this);
}

void dae::SoundManager::NotifySound(SoundData soundData)
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	soundData.filePath = m_dataPath + soundData.filePath;
	m_EventQueue.push(soundData);

	m_QueueCondition.notify_one();
}

dae::SoundManager::SoundManager()
{
}

dae::SoundManager::~SoundManager()
{
	m_ThreadRunning = false;
}

void dae::SoundManager::PlaySound(SoundData soundData)
{
	if (!m_SoundSystem->IsSoundLoaded(soundData.id))
	{
		m_SoundSystem->LoadSound(soundData.id, soundData.filePath);
	}

	m_SoundSystem->PlaySound(soundData.id, static_cast<SoundSystem::SoundType>(soundData.soundType), soundData.volume);
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
		m_QueueCondition.wait(lock, [this] { return !m_EventQueue.empty(); });

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