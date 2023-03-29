#pragma once

namespace dae
{
	class GameObject;
	class Command
	{
	public:

		enum class ButtonState
		{
			Up,
			Down,
			Pressed
		};
		virtual ~Command() = default;

		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		virtual void Execute() = 0;

		void SetButtonsState(ButtonState  buttonState) { m_ButtonsState = buttonState; };
		ButtonState GetButtonsState() { return m_ButtonsState; };

	protected:

		explicit Command() = default;

	private:

		ButtonState m_ButtonsState{ ButtonState::Pressed };

	};

}
