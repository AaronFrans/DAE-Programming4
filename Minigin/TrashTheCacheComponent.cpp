#include "TrashTheCacheComponent.h"
#include <chrono>
#include <numeric>
#include <algorithm>

using namespace std::chrono;


dae::TrashTheCacheComponent::TrashTheCacheComponent(std::weak_ptr<GameObject> owner)
	: Component(owner)
{
}

void dae::TrashTheCacheComponent::RenderImGui()
{
	RenderExcersize1();

	RenderExcersize2();
}

void dae::TrashTheCacheComponent::RenderExcersize1()
{

	ImGui::Begin("Excersise 1", nullptr);

	ImGui::InputInt("Nr Of Runs", &m_NrOfRunsEx1);

	if (ImGui::Button("Calulate ints"))
		MeasureInts();


	if (m_IntResults.size() > 0)
		RenderPlot("Ints", m_IntResults);


	ImGui::End();
}

void dae::TrashTheCacheComponent::RenderExcersize2()
{

	ImGui::Begin("Excersise 2", nullptr);

	ImGui::InputInt("Nr of Runs", &m_NrOfRunsEx2);

	if (ImGui::Button("Measure GameObjects"))
		MeasureGameObjects3D();

	if (m_GameObjectsResults.size() > 0)
		RenderPlot("GameObjects", m_GameObjectsResults);

	ImGui::Separator();

	if (ImGui::Button("Measure GameObjectAlts"))
		MeasureGameObject3DAlts();

	if (m_GameObjectAltsResults.size() > 0)
		RenderPlot("GameObjectAlts", m_GameObjectAltsResults);

	ImGui::Separator();

	if (m_GameObjectsResults.size() > 0 && m_GameObjectAltsResults.size() > 0)
		RenderDoublePlot("Normal vs Alts", m_GameObjectsResults, m_GameObjectAltsResults);

	ImGui::End();
}

void dae::TrashTheCacheComponent::MeasureInts()
{
	m_IntResults.clear();

	int* arr = new int[ARRAY_SIZE] {1};

	std::map<unsigned int, std::vector<int>> timings{};

	for (int sample = 0; sample < m_NrOfRunsEx1; sample++)
	{
		for (unsigned int steps = 1; steps <= 1024; steps *= 2)
		{
			auto start = high_resolution_clock::now();

			for (unsigned int i = 0; i < ARRAY_SIZE; i += steps)
			{
				arr[i] *= 2;
			}

			auto end = high_resolution_clock::now();

			auto elapsed = duration_cast<microseconds>(end - start);

			timings[steps].push_back(static_cast<int>(elapsed.count()));
		}

	}

	HandleTimings(timings, m_IntResults);

	delete[] arr;
}

void dae::TrashTheCacheComponent::MeasureGameObjects3D()
{
	m_GameObjectsResults.clear();
	GameObject3D* arr = new GameObject3D[ARRAY_SIZE];

	std::map<unsigned int, std::vector<int>> timings{};

	for (int sample = 0; sample < m_NrOfRunsEx2; sample++)
	{
		for (unsigned int steps = 1; steps <= 1024; steps *= 2)
		{
			auto start = high_resolution_clock::now();

			for (unsigned int i = 0; i < ARRAY_SIZE; i += steps)
			{
				arr[i].m_Id *= 2;
			}

			auto end = high_resolution_clock::now();

			auto elapsed = duration_cast<microseconds>(end - start);

			timings[steps].push_back(static_cast<int>(elapsed.count()));
		}

	}

	HandleTimings(timings, m_GameObjectsResults);
	delete[] arr;
}

void dae::TrashTheCacheComponent::MeasureGameObject3DAlts()
{
	m_GameObjectAltsResults.clear();
	GameObject3DAlt* arr = new GameObject3DAlt[ARRAY_SIZE];

	std::map<unsigned int, std::vector<int>> timings{};

	for (int sample = 0; sample < m_NrOfRunsEx2; sample++)
	{
		for (unsigned int steps = 1; steps <= 1024; steps *= 2)
		{
			auto start = high_resolution_clock::now();

			for (unsigned int i = 0; i < ARRAY_SIZE; i += steps)
			{
				arr[i].m_Id *= 2;
			}

			auto end = high_resolution_clock::now();

			auto elapsed = duration_cast<microseconds>(end - start);

			timings[steps].push_back(static_cast<int>(elapsed.count()));
		}


	}

	HandleTimings(timings, m_GameObjectAltsResults);
	delete[] arr;
}


std::vector<int> dae::TrashTheCacheComponent::WithoutHiLo(const std::vector<int>& orig)
{
	//taking copy to keep original vector the same
	std::vector<int> copy = orig;

	std::sort(copy.begin(), copy.end());
	return std::vector(&copy[1], &copy[copy.size() - 1]);
}


float dae::TrashTheCacheComponent::Average(const std::vector<int>& v)
{
	if (v.empty()) {
		return 0;
	}

	auto const count = static_cast<float>(v.size());

	return std::reduce(v.begin(), v.end()) / count;
}

void dae::TrashTheCacheComponent::HandleTimings(const std::map<unsigned int, std::vector<int>>& timings, std::vector<float>& resultVec)
{
	for (auto& timing : timings)
	{
		auto data = WithoutHiLo(timing.second);
		auto average = Average(data);

		resultVec.push_back(average);
	}
}

void dae::TrashTheCacheComponent::RenderPlot(const char* label, const std::vector<float>& timings)
{

	float maxVal = *std::max_element(begin(timings), end(timings));
	float minVal = *std::min_element(begin(timings), end(timings));



	ImGui::PlotConfig conf;
	conf.values.color = FIRST_COLOR;
	conf.values.ys = timings.data();
	conf.values.xs = TIME_STEPS.data();
	conf.values.count = static_cast<int>(timings.size() + 1);
	conf.scale.min = minVal;
	conf.scale.max = maxVal;
	conf.tooltip.show = true;
	conf.tooltip.format = "x=%.2f, y=%.2f";
	conf.grid_x.show = true;
	conf.grid_y.show = true;
	conf.grid_y.size = maxVal / 10.0f;
	conf.frame_size = ImVec2(200, 200);
	conf.line_thickness = 2.f;

	ImGui::Plot(label, conf);
}

void dae::TrashTheCacheComponent::RenderDoublePlot(const char* label, const std::vector<float>& timings1, const std::vector<float>& timings2)
{
	const float maxVal = std::max(*std::max_element(begin(timings1), end(timings1)), *std::max_element(begin(timings2), end(timings2)));
	const float minVal = std::min(*std::min_element(begin(timings1), end(timings1)), *std::min_element(begin(timings2), end(timings2)));

	const int nrOfPoints = static_cast<int>(std::max(timings1.size() + 1, timings2.size() + 1));
	const float* timingsGroup[2] = {timings1.data(), timings2.data()};
	 
	const ImU32 colors[2] = { FIRST_COLOR, SECOND_COLOR };

	ImGui::PlotConfig conf;
	conf.values.colors = colors;
	conf.values.ys_list = timingsGroup;
	conf.values.ys_count = 2;
	conf.values.xs = TIME_STEPS.data();
	conf.values.count = nrOfPoints;
	conf.scale.min = minVal;
	conf.scale.max = maxVal;
	conf.tooltip.show = true;
	conf.tooltip.format = "x=%.2f, y=%.2f";
	conf.grid_x.show = true;
	conf.grid_y.show = true;
	conf.grid_y.size = maxVal / 10.0f;
	conf.frame_size = ImVec2(200, 200);
	conf.line_thickness = 2.f;

	ImGui::Plot(label, conf);
}

