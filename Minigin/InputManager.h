#pragma once
#include <utility>
#include <map>
#include <vector>
#include <memory>
#include <stdexcept>

#include "Singleton.h"
#include "XboxController.h"
#include "Command.h"


namespace dae
{


	class InputManager final : public Singleton<InputManager>
	{

	public:
		//bind a controller index to a button
		using ControllerKey = std::pair<unsigned, XboxController::ControllerButton>;
		//keep track of all buttons with commands
		using ControllerCommandsMap = std::map<ControllerKey, std::unique_ptr<Command>>;

		//keep track of all buttons with commands
		using KeyboardCommandsMap = std::map<SDL_Scancode, std::unique_ptr<Command>>;



		bool ProccesCommands();

		unsigned AddController();

		template<typename T> T* AddXboxCommand(std::unique_ptr<T> command, unsigned controllerIndex, XboxController::ControllerButton buttonToPress);

		template<typename T> T* AddKeyboardCommand(std::unique_ptr<T> command, SDL_Scancode keyToPress);

	private:
		void HandleConrollerInputs();

		void HandleKeyboardInputs();

		bool ProcessInput();

		void HandleKeyboardKey(Command::ButtonState state, SDL_Scancode scancode);

		//make collections of commands
		ControllerCommandsMap m_ControllerCommands{ };
		KeyboardCommandsMap m_KeyboardCommands{ };


		//collection with all controllers
		std::vector<std::unique_ptr<XboxController>> m_Controllers{};

		const Uint8* m_pCurrentState{ SDL_GetKeyboardState(NULL)};

		std::vector<bool> m_UpKeys{ std::vector<bool>(322, false) };
		std::vector<bool> m_DownKeys{ std::vector<bool>(322, false) };
	};

	template<typename T>
	inline T* InputManager::AddXboxCommand(std::unique_ptr<T> command, unsigned controllerIndex, XboxController::ControllerButton buttonToPress)
	{
		static_assert(std::is_base_of<Command, T>::value && "T must inherit from Command");

		while (controllerIndex >= m_Controllers.size())
		{
			AddController();
		}


		T* toReturn = command.get();


		ControllerKey controllerKeyToPress = std::make_pair(controllerIndex, buttonToPress);

		m_ControllerCommands.insert({ controllerKeyToPress , std::move(command) });

		return toReturn;
	}

	template<typename T>
	inline T* InputManager::AddKeyboardCommand(std::unique_ptr<T> command, SDL_Scancode keyToPress)
	{
		static_assert(std::is_base_of<Command, T>::value && "T must inherit from Command");



		T* toReturn = command.get();

		m_KeyboardCommands.insert({ keyToPress , std::move(command) });

		return toReturn;
	}

}
