#pragma once
#include <string>

namespace dae
{


	struct Event
	{
		virtual ~Event() = default;

		const char* eventType{};
	};

	struct PlayerEvent : Event
	{
		unsigned playerIndex{};
	};

	inline bool operator==(const Event& lhs, const Event& rhs)
	{
		return lhs.eventType == rhs.eventType;
	}
}

namespace std
{
	template<>
	struct hash<dae::Event>
	{
		std::size_t operator()(const dae::Event& input) const
		{
			return std::hash<const char*>{}(input.eventType);
		}
	};
}