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
		explicit Command(GameObject* pActor);

		GameObject* GetActor() const { return m_pActor; };

	private:
		//don't delete -> not owned
		GameObject* m_pActor{ nullptr };
		
	};

}
