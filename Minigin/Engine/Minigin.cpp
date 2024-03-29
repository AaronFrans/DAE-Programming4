#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <thread>

#include "Minigin.h"

#include "Input/InputManager.h"
#include "Scene/SceneManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/ResourceManager.h"
#include "Timer.h"
#include "Events/EventManager.h"

#include "Sounds/SoundManager.h"
#include "Sounds/SDLSoundSystem.h"
#include <iostream>

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
		printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
			version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string& dataPath, unsigned windowWidth, unsigned windowHeight)
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);

	auto& sounds = SoundManager::GetInstance();
	sounds.Init(dataPath);

	sounds.SetSoundSystem(std::make_unique<SDLSoundSystem>());
}

dae::Minigin::~Minigin()
{
	SceneManager::GetInstance().Quit();
	Renderer::GetInstance().Destroy();
	SoundManager::GetInstance().Quit();
	InputManager::GetInstance().Quit();
	EventManager::GetInstance().Quit();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& timer = Timer::GetInstance();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();

	auto& eventHandler = EventManager::GetInstance();


	bool doContinue = true;

	float lag{ 0 };
	while (doContinue)
	{
		lag += timer.GetDeltaTime();


		timer.Update();
		doContinue = input.ProccesCommands();

		while (lag >= timer.GetTimeStep())
		{
			sceneManager.Update();
			lag -= timer.GetTimeStep();
		}

		renderer.Render();


		eventHandler.HandleEvents();

		//sleep -> expected time - time for update + rendertime
		auto sleepTime = timer.GetLastTimeStamp() + std::chrono::milliseconds(timer.GetFrameTime()) - std::chrono::high_resolution_clock::now();

		std::this_thread::sleep_for(sleepTime);
	}
}
