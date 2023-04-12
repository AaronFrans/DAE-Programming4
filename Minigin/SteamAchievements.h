#pragma once
#include <stdint.h>
#include <isteamuserstats.h>
#include <steam_gameserver.h>
#include <vector>

namespace dae
{

	enum EAchievements
	{
		ACH_WIN_ONE_GAME = 0,
		ACH_WIN_100_GAMES = 1,
		ACH_TRAVEL_FAR_ACCUM = 2,
		ACH_TRAVEL_FAR_SINGLE = 3,
	};

	struct Achievement
	{

		Achievement(int achievementID, const char* chAchievementID)
			: m_eAchievementID{ achievementID }
			, m_pchAchievementID{ chAchievementID }
		{};

		int m_eAchievementID{};
		const char* m_pchAchievementID{};
		char m_rgchName[128]{};
		char m_rgchDescription[256]{};
		bool m_bAchieved{};
		int m_iIconImage{};
	};

	class SteamAchievements
	{

	public:
		SteamAchievements(const std::vector<Achievement>& Achievements);

		bool RequestStats();
		bool SetAchievement(const char* ID);
		void HasReceivedAchievement(const char* ID, bool* hasAchieved);

		void ClearAchievements();


		STEAM_CALLBACK(SteamAchievements, OnUserStatsReceived, UserStatsReceived_t,
			m_CallbackUserStatsReceived);
		STEAM_CALLBACK(SteamAchievements, OnUserStatsStored, UserStatsStored_t,
			m_CallbackUserStatsStored);
		STEAM_CALLBACK(SteamAchievements, OnAchievementStored,
			UserAchievementStored_t, m_CallbackAchievementStored);


	private:
		uint64_t m_AppID; // Our current AppID
		std::vector<Achievement> m_Achievements; // Achievements data
		int m_NumAchievements; // The number of Achievements
		bool m_bInitialized; // Have we called Request stats and received the callback?
	};

}


