
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

	void PlaySound(unsigned short id, SoundType soundType, float volume);

	void LoadSound(unsigned short id, const std::string& filepath);

	bool IsSoundLoaded(unsigned short id);


	void Quit();
private:

	std::unordered_map<unsigned short, Mix_Chunk*> m_LoadedSounds;
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

void SDLSoundSystem::SDLMixerImpl::PlaySound(unsigned short id, SoundType soundType, float volume)
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
	case dae::SDLSoundSystem::SoundType::SoundEffect:
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

void SDLSoundSystem::SDLMixerImpl::Quit()
{
	for (auto& sound : m_LoadedSounds)
	{
		Mix_FreeChunk(sound.second);
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
}

void SDLSoundSystem::Init()
{
	m_pImpl = std::make_unique<SDLMixerImpl>();
	m_pImpl->Init();
}

void dae::SDLSoundSystem::Quit()
{
	if (m_pImpl)
	{
		m_pImpl->Quit();
	}
}

void SDLSoundSystem::PlaySound(unsigned short id, SoundType soundType, float volume)
{
	m_pImpl->PlaySound(id, soundType, volume);
}

void SDLSoundSystem::LoadSound(unsigned short id, const std::string& filepath)
{
	m_pImpl->LoadSound(id, filepath);
}

bool dae::SDLSoundSystem::IsSoundLoaded(unsigned short id)
{
	return m_pImpl->IsSoundLoaded(id);
}
#pragma endregion



