#pragma once
#include "Component.h"
#include <map>
#include <vector>

namespace dae
{

	struct Transform
	{
		float matrix[16]
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1 };
	};

	class GameObject3D
	{
	public:

		Transform m_Transform{};
		int m_Id{};
	};

	class GameObject3DAlt
	{
	public:

		~GameObject3DAlt() { delete m_pTransform; }
		Transform* m_pTransform{};
		int m_Id{};
	};


	class TrashTheCacheComponent :
		public Component
	{
	public:

		TrashTheCacheComponent(std::weak_ptr<GameObject> owner);

		~TrashTheCacheComponent() = default;
		TrashTheCacheComponent(const TrashTheCacheComponent& other) = delete;
		TrashTheCacheComponent(TrashTheCacheComponent&& other) = delete;
		TrashTheCacheComponent& operator=(const TrashTheCacheComponent& other) = delete;
		TrashTheCacheComponent& operator=(TrashTheCacheComponent&& other) = delete;


		void RenderImGui() override;

	private:
		const unsigned int ARRAY_SIZE = 10'000'000;
		const std::vector<float> TIME_STEPS{ 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };

		void RenderExercise1();
		void RenderExercise2();


		std::vector<int> WithoutHiLo(const std::vector<int>& orig);
		float Average(const std::vector<int>& v);

		void HandleTimings(const std::map<int, std::vector<int>>& timings, std::vector<float>& resultVec);

		void Exercise1();


		void GameObjects();
		void GameObjectAlts();


		void PlotTimes(const std::vector<float>& times, const char* label);


		std::vector<float> m_Ex1Times{};
		int m_Ex1NrRuns{ 10 };


		std::vector<float> m_Ex2AltTimes{};
		std::vector<float> m_Ex2Times{};
		int m_Ex2NrRuns{ 10 };

	};
}

