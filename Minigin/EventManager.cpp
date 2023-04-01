#include "EventManager.h"
#include <iostream>
void dae::EventManager::SendEventMessage(const Event& e)
{
	const unsigned currentIndex = (m_BufferStart + m_NrQueued) & BUFFER_SIZE;
	m_Queue[currentIndex] = e;

	++m_NrQueued;
}

void dae::EventManager::AddObserver(const Event& e, const std::function<void()>& observer)
{
	auto& observerIt = m_Observers[e];
	observerIt.push_back(observer);
}

void dae::EventManager::HandleEvents()
{
	Event e{};
	while (PollEvents(e))
	{
		for (const auto& observer :m_Observers[e])
		{
			observer();
		}
	}
}

bool dae::EventManager::PollEvents(Event& e)
{
	if (m_NrQueued == 0) 
		return false;

	e = m_Queue[m_BufferStart];

	m_BufferStart = m_BufferStart + 1 % BUFFER_SIZE;
	--m_NrQueued;

	return true;
}
