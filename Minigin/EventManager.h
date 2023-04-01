#pragma once
#include "Singleton.h"
#include "Event.h"

#include <unordered_map>
#include <vector>
#include <functional>

namespace dae
{
	class EventManager final : public Singleton<EventManager>
	{
	public:
		void SendEventMessage(const Event& e);

		void AddObserver(const Event& e, const std::function<void()>& observer);

		void HandleEvents();

	private:

		bool PollEvents(Event& e);

		const static unsigned BUFFER_SIZE{ 20 };

		Event m_Queue[BUFFER_SIZE]{};
		unsigned m_BufferStart{};
		unsigned m_NrQueued{};

		std::unordered_map<Event, std::vector<std::function<void()>>> m_Observers{};
	};
}

