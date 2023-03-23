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


	class InputManager final : public Singleton<InputManager>
	{

	public:
		bool ProcessInput();

		unsigned AddController();

		template<typename T> T* AddCommand(unsigned controllerIndex, GameObject* actor, XboxController::ControllerButton buttonToPress);

	private:
		void HandleConrollerInputs();


		//make collection of commands
		ControllerCommandsMap m_ControllerCommands{ };


		//collection with all controllers
		std::vector<std::unique_ptr<XboxController>> m_Controllers{};

	};

	template<typename T>
	inline T* InputManager::AddCommand(unsigned controllerIndex, GameObject* actor, XboxController::ControllerButton buttonToPress)
	{
		static_assert(std::is_base_of<Command, T>::value && "T must inherit from Command");

		while (controllerIndex >= m_Controllers.size())
		{
			AddController();
		}

		std::unique_ptr<T> command = std::make_unique<T>(actor);
		T* toReturn = command.get();
		ControllerKey keyToPress = std::make_pair( controllerIndex, buttonToPress );

		m_ControllerCommands.insert({ keyToPress , std::move(command)});

		return toReturn;
	}

}
