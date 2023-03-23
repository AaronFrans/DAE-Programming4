#include "TrashTheCacheComponent.h"
#include <imgui_plot.h>
#include <imgui.h>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <thread>



using namespace std::chrono;

dae::TrashTheCacheComponent::TrashTheCacheComponent(std::weak_ptr<GameObject> owner) : Component(owner)
{
}

void dae::TrashTheCacheComponent::RenderImGui()
{
	RenderExercise1();

	RenderExercise2();
}

void dae::TrashTheCacheComponent::Exercise1()
{

	m_Ex1Times.clear();
	int* arr = new int[ARRAY_SIZE];

	std::map<int, std::vector<int>> timings{};

	for (size_t sample = 0; sample < 20; sample++)
	{

		for (int steps = 1; steps <= 1024; steps *= 2)
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

	HandleTimings(timings, m_Ex1Times);

	delete[] arr;
}

void dae::TrashTheCacheComponent::GameObjects()
{

	m_Ex2Times.clear();
	GameObject3D* arr = new GameObject3D[ARRAY_SIZE];

	std::map<int, std::vector<int>> timings{};

	for (size_t sample = 0; sample < 20; sample++)
	{
		for (int steps = 1; steps <= 1024; steps *= 2)
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

	HandleTimings(timings, m_Ex2Times);
	delete[] arr;
}

void dae::TrashTheCacheComponent::GameObjectAlts()
{
	m_Ex2AltTimes.clear();
	GameObject3DAlt* arr = new GameObject3DAlt[ARRAY_SIZE];

	std::map<int, std::vector<int>> timings{};

	for (size_t sample = 0; sample < 20; sample++)
	{
		for (int steps = 1; steps <= 1024; steps *= 2)
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

	HandleTimings(timings, m_Ex2AltTimes);
	delete[] arr;
}

void dae::TrashTheCacheComponent::PlotTimes(const std::vector<float>& times, const char* label)
{
	ImColor color{ 1.f, 0.f, 0.f, 1.f };

	auto maxVal = *std::max_element(begin(times), end(times));
	auto minVal = *std::min_element(begin(times), end(times));

	ImGui::PlotConfig conf;
	conf.values.ys = times.data();
	conf.values.xs = TIME_STEPS.data();
	conf.values.count = static_cast<int>(times.size() + 1);
	conf.values.color = color;

	conf.scale.min = minVal;
	conf.scale.max = maxVal;

	conf.tooltip.show = true;
	conf.tooltip.format = "x=%.2f, y=%.2f";

	conf.grid_x.show = true;
	conf.grid_y.show = true;

	conf.frame_size = ImVec2(200, 200);
	conf.line_thickness = 2.f;

	ImGui::Plot(label, conf);
}

void dae::TrashTheCacheComponent::RenderExercise1()
{
	ImGui::Begin("Exercise 1", nullptr);

	ImGui::InputInt("Nr of runs", &m_Ex1NrRuns);

	if (ImGui::Button("Ints"))
		Exercise1();


	if (m_Ex1Times.size() > 0)
	{
		ImGui::SeparatorText("Results for ints");
		PlotTimes(m_Ex1Times, "Ex1");
	}


	ImGui::End();
}

void dae::TrashTheCacheComponent::RenderExercise2()
{
	ImGui::Begin("Exercise 2", nullptr);

	ImGui::InputInt("Nr of runs", &m_Ex2NrRuns);

	if (ImGui::Button("GameObejects"))
		GameObjects();

	if (ImGui::Button("GameObejectAlts"))
		GameObjectAlts();


	if (m_Ex2Times.size() > 0)
	{
		ImGui::SeparatorText("Results for GameObjects");
		PlotTimes(m_Ex2Times, "Ex2");

	}
	if (m_Ex2AltTimes.size() > 0)
	{
		ImGui::SeparatorText("Results for GameObjectAlts");
		PlotTimes(m_Ex2AltTimes, "Ex2Alt");

	}
	if (m_Ex2Times.size() > 0 && m_Ex2AltTimes.size() > 0)
	{
		ImGui::SeparatorText("Combined Graphs");


		ImU32 colors[2]{ ImColor{1.f, 0.f, 0.f}, ImColor{0.f, 1.f, 1.f} };
		const float* doubleValues[2]{ m_Ex2Times.data(), m_Ex2AltTimes.data() };


		auto maxVal = std::max(*std::max_element(begin(m_Ex2Times), end(m_Ex2Times)),
			*std::max_element(begin(m_Ex2AltTimes), end(m_Ex2AltTimes)));
		auto minVal = std::min(*std::min_element(begin(m_Ex2Times), end(m_Ex2Times)),
			*std::min_element(begin(m_Ex2AltTimes), end(m_Ex2AltTimes)));

		ImGui::PlotConfig conf;
		conf.values.ys_list = doubleValues;
		conf.values.xs = TIME_STEPS.data();
		conf.values.count = static_cast<int>(m_Ex2Times.size() + 1);
		conf.values.ys_count = 2;
		conf.values.colors = colors;

		conf.scale.min = minVal;
		conf.scale.max = maxVal;

		conf.tooltip.show = true;
		conf.tooltip.format = "x=%.2f, y=%.2f";

		conf.grid_x.show = true;
		conf.grid_y.show = true;

		conf.frame_size = ImVec2(200, 200);

		conf.line_thickness = 2.f;

		ImGui::Plot("plot", conf);
	}

	ImGui::End();
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

void dae::TrashTheCacheComponent::HandleTimings(const std::map<int, std::vector<int>>& timings, std::vector<float>& resultVec)
{
	for (auto& timing : timings)
	{
		auto data = WithoutHiLo(timing.second);
		auto average = Average(data);

		resultVec.push_back(average);
	}
}

