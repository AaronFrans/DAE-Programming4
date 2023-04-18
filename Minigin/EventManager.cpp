#include "EventManager.h"
#include <iostream>


void dae::EventManager::AddObserver(const Event& e, const std::function<void(const Event* e)>& observer)
{
	auto& observerIt = m_Observers[e];
	observerIt.push_back(observer);
}

void dae::EventManager::HandleEvents()
{
	Event* e{ nullptr };
	while (PollEvents(e))
	{
		for (const auto& observer : m_Observers[*e])
		{
			observer(e);
		}
	}
}

bool dae::EventManager::PollEvents(Event*& e)
{
	if (m_NrQueued == 0)
		return false;

	e = m_Queue[m_BufferStart].get();

	++m_BufferStart %= BUFFER_SIZE;
	--m_NrQueued;

	return true;
}
