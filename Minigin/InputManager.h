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
	//bind a controller index to a button
	using ControllerKey = std::pair<unsigned, XboxController::ControllerButton>;
	//keep track of all buttons with commands
	using ControllerCommandsMap = std::map<ControllerKey, std::unique_ptr<Command>>;

	//keep track of all buttons with commands
	using KeyboardCommandsMap = std::map<SDL_Scancode, std::unique_ptr<Command>>;


	class InputManager final : public Singleton<InputManager>
	{

	public:

		bool ProccesCommands();

		unsigned AddController();

		template<typename T> T* AddXboxCommand(unsigned controllerIndex, GameObject* actor, XboxController::ControllerButton buttonToPress);

		template<typename T> T* AddKeyboardCommand(GameObject* actor, SDL_Scancode keyToPress);

	private:
		void HandleConrollerInputs();

		void HandleKeyboardInputs();

		bool ProcessInput();

		//make collections of commands
		ControllerCommandsMap m_ControllerCommands{ };
		KeyboardCommandsMap m_KeyboardCommands{ };


		//collection with all controllers
		std::vector<std::unique_ptr<XboxController>> m_Controllers{};



		// implementation if keyboard via: https://stackoverflow.com/questions/3741055/inputs-in-sdl-on-key-pressed
		std::vector<bool> m_PressedKeys{ std::vector<bool>(322, false) };



	};

	template<typename T>
	inline T* InputManager::AddXboxCommand(unsigned controllerIndex, GameObject* actor, XboxController::ControllerButton buttonToPress)
	{
		static_assert(std::is_base_of<Command, T>::value && "T must inherit from Command");

		while (controllerIndex >= m_Controllers.size())
		{
			AddController();
		}

		std::unique_ptr<T> command = std::make_unique<T>(actor);
		T* toReturn = command.get();
		ControllerKey controllerKeyToPress = std::make_pair( controllerIndex, buttonToPress );

		m_ControllerCommands.insert({ controllerKeyToPress , std::move(command)});

		return toReturn;
	}

	template<typename T>
	inline T* InputManager::AddKeyboardCommand(GameObject* actor, SDL_Scancode keyToPress)
	{
		static_assert(std::is_base_of<Command, T>::value && "T must inherit from Command");


		std::unique_ptr<T> command = std::make_unique<T>(actor);
		T* toReturn = command.get();

		m_KeyboardCommands.insert({ keyToPress , std::move(command)});

		return toReturn;
	}

}
