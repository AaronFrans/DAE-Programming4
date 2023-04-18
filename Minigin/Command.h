#pragma once

namespace dae
{
	class GameObject;
	class Command
	{
	public:


		virtual ~Command() = default;

		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

		virtual void Execute() = 0;

	protected:

		explicit Command() = default;

	private:


	};

}
