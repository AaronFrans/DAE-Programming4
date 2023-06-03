#pragma once
#include "Components/Component.h"
#include "imgui_plot.h"
#include "imgui.h"
#include <map>

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

	class TrashTheCacheComponent final :
		public Component
	{

	public:
		TrashTheCacheComponent(GameObject* owner);

		~TrashTheCacheComponent() = default;
		TrashTheCacheComponent(const TrashTheCacheComponent& other) = delete;
		TrashTheCacheComponent(TrashTheCacheComponent&& other) = delete;
		TrashTheCacheComponent& operator=(const TrashTheCacheComponent& other) = delete;
		TrashTheCacheComponent& operator=(TrashTheCacheComponent&& other) = delete;

		void RenderImGui() override;

	private:

		std::vector<int> WithoutHiLo(const std::vector<int>& orig);
		float Average(const std::vector<int>& v);
		void HandleTimings(const std::map<unsigned int, std::vector<int>>& timings, std::vector<float>& resultVec);

		void RenderPlot(const char* label, const std::vector<float>& timings);
		void RenderDoublePlot(const char* label, const std::vector<float>& timings1, const std::vector<float>& timings2);

		void RenderExcersize1();
		void RenderExcersize2();


		void MeasureInts();
		void MeasureGameObjects3D();
		void MeasureGameObject3DAlts();


		int m_NrOfRunsEx1{ 10 };
		int m_NrOfRunsEx2{ 10 };

		std::vector<float> m_IntResults{};
		std::vector<float> m_GameObjectsResults{};
		std::vector<float> m_GameObjectAltsResults{};

		const unsigned int ARRAY_SIZE{ 10'000'000 };
		const std::vector<float> TIME_STEPS{ 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
		const ImColor FIRST_COLOR{ ImColor{1.0f, 0.0f, 0.0f, 1.0f} };
		const ImColor SECOND_COLOR{ ImColor{0.0f, 1.0f, 0.0f, 1.0f} };
#
	};
}
