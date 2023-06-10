
#include <stdexcept>
#include <unordered_map>

#include "SDL.h"
#include "SDL_mixer.h"

#include "SDLSoundSystem.h"

#define MIX_CHANNEL_GROUP_MUSIC 0
#define MIX_CHANNEL_GROUP_EFFECTS 1

using namespace dae;
#pragma region SDLSound Implemntation

class SDLSoundSystem::SDLMixerImpl final
{
public:
	void Init();

	void PlaySound(unsigned short id, SoundData::SoundType soundType, float volume);

	void LoadSound(unsigned short id, const std::string& filepath);

	bool IsSoundLoaded(unsigned short id);

	void Mute();

	void Quit();
private:

	std::unordered_map<unsigned short, Mix_Chunk*> m_LoadedSounds;

	bool m_IsMuted{ false };

};

void SDLSoundSystem::SDLMixerImpl::Init()
{
	SDL_version version{};

	SDL_MIXER_VERSION(&version)
		printf("We compiled against SDL_mixer version %u.%u.%u ...\n",
			version.major, version.minor, version.patch);

	version = *Mix_Linked_Version();
	printf("We are linking against SDL_mixer version %u.%u.%u.\n",
		version.major, version.minor, version.patch);


	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		throw std::runtime_error(std::string("Failed to load support for audio: ") + SDL_GetError());
	}

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 5, 0) < 0) {
		throw std::runtime_error("Failed to open audio: " + std::string(Mix_GetError()));
	}


	Mix_GroupChannels(0, 0, MIX_CHANNEL_GROUP_MUSIC);       // Background music group (1 channel)
	Mix_GroupChannels(1, 4, MIX_CHANNEL_GROUP_EFFECTS);		// SoundEffects music group (4 channels)

}

void SDLSoundSystem::SDLMixerImpl::PlaySound(unsigned short id, SoundData::SoundType soundType, float volume)
{
	if (!IsSoundLoaded(id))
		throw std::runtime_error("Sound not found. Please load the sound first.");

	Mix_Chunk* chunk = m_LoadedSounds[id];

	int mixVolume = static_cast<int>(volume * MIX_MAX_VOLUME); // Convert float volume to SDL_mixer scale
	Mix_VolumeChunk(chunk, mixVolume);


	int channel{ 0 };
	int repeat{ 1 };
	switch (soundType)
	{
	case SoundData::SoundType::SoundEffect:
		channel = Mix_GroupAvailable(MIX_CHANNEL_GROUP_EFFECTS);
		repeat = 0;
		// if no channel available use channel 1
		if (channel == -1)
		{
			channel = 1;
		}
		break;
	}


	Mix_PlayChannel(channel, chunk, repeat);

}

void SDLSoundSystem::SDLMixerImpl::LoadSound(unsigned short id, const std::string& filepath)
{
	if (!IsSoundLoaded(id))
	{
		Mix_Chunk* chunk = Mix_LoadWAV(filepath.c_str());
		if (chunk == nullptr)
		{
			throw std::runtime_error(std::string("Failed to load sound: ") + Mix_GetError());
		}

		m_LoadedSounds[id] = chunk;
	}
}

bool SDLSoundSystem::SDLMixerImpl::IsSoundLoaded(unsigned short id)
{
	return m_LoadedSounds.find(id) != m_LoadedSounds.end();
}

void SDLSoundSystem::SDLMixerImpl::Mute()
{
	if (m_IsMuted)
	{
		for (int i = 0; i < 5; i++)
		{
			Mix_Volume(i, 128);
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			Mix_Volume(i, 0);
		}
	}

	m_IsMuted = !m_IsMuted;
}

void SDLSoundSystem::SDLMixerImpl::Quit()
{
	for (auto& sound : m_LoadedSounds)
	{
		if (sound.second == nullptr)
		{
			Mix_FreeChunk(sound.second);
			sound.second = nullptr;
		}
	}

	Mix_CloseAudio();
	Mix_Quit();
}

#pragma endregion


#pragma region Public SDLSoundSystem

SDLSoundSystem::SDLSoundSystem()
	:SoundSystem()
{
}

SDLSoundSystem::~SDLSoundSystem()
{
	Quit();
}

void SDLSoundSystem::Init(const std::string& dataPath)
{
	m_pImpl = std::make_unique<SDLMixerImpl>();
	m_DataPath = dataPath;
	m_pImpl->Init();


	m_ThreadRunning = true;
	m_SoundThread = std::jthread(&SDLSoundSystem::SoundThread, this);

}

void dae::SDLSoundSystem::Mute()
{
	m_pImpl->Mute();
}

void dae::SDLSoundSystem::Quit()
{

	m_ThreadRunning = false;

	m_QueueCondition.notify_all();

	if (m_pImpl)
	{
		m_pImpl->Quit();
	}
}

void dae::SDLSoundSystem::HandleSoundData(SoundData soundData)
{

	std::lock_guard<std::mutex> lock(m_QueueMutex);
	soundData.filePath = m_DataPath + soundData.filePath;
	m_EventQueue.push(soundData);

	m_QueueCondition.notify_all();
}

void SDLSoundSystem::PlaySound(const SoundData& soundData)
{
	if (!IsSoundLoaded(soundData.id))
	{
		LoadSound(soundData);
	}


	m_pImpl->PlaySound(soundData.id, soundData.soundType, soundData.volume);
}

void SDLSoundSystem::LoadSound(const SoundData& soundData)
{
	if (!IsSoundLoaded(soundData.id))
	{
		m_pImpl->LoadSound(soundData.id, soundData.filePath);
	}

}

bool dae::SDLSoundSystem::IsSoundLoaded(unsigned short id)
{
	return m_pImpl->IsSoundLoaded(id);
}
void dae::SDLSoundSystem::SoundThread()
{
	while (m_ThreadRunning)
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);
		m_QueueCondition.wait(lock, [&] {

			if (!m_ThreadRunning)
				return true;

			return !m_EventQueue.empty();
			});


		if (m_EventQueue.empty())
			return;

		SoundData data = m_EventQueue.front();


		m_EventQueue.pop();
		lock.unlock();

		if (data.loadFile)
			LoadSound(data);
		else
			PlaySound(data);

	}
}
#pragma endregion



