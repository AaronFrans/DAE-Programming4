#pragma once
#include "Singleton.h"
#include "Event.h"

#include <unordered_map>
#include <vector>
#include <functional>
#include <memory>

namespace dae
{
	class EventManager final : public Singleton<EventManager>
	{
	public:

		template <typename T>
		void SendEventMessage(std::unique_ptr<T> e);

		void AddObserver(const Event& e, const std::function<void(const Event* e)>& observer);

		void RemoveObserver(const Event& e, const std::function<void(const Event* e)>& observer);

		void HandleEvents();

	private:

		bool PollEvents(Event*& e);

		const static unsigned BUFFER_SIZE{ 20 };

		std::vector<std::unique_ptr<Event>> m_Queue{ BUFFER_SIZE };
		unsigned m_BufferStart{};
		unsigned m_NrQueued{};

		std::unordered_map<Event, std::vector<std::function<void(const Event* e)>>> m_Observers{};
	};


	template<typename T>
	inline void EventManager::SendEventMessage(std::unique_ptr<T>  e)
	{
		static_assert(std::is_base_of<Event, T>::value && "T must inherit from Event");

		const unsigned currentIndex = (m_BufferStart + m_NrQueued) % BUFFER_SIZE;
		m_Queue[currentIndex] = std::move(e);

		++m_NrQueued;
	}

}

