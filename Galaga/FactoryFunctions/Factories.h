#pragma once
#include <string>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace dae
{
	enum class EnemyTypes
	{
		Bee = 0,
		Butterfly,
		Boss
	};


	constexpr unsigned g_WindowWidth{ 640 }, g_WindowHeight{ 480 };



	class GameObject;
	class TransformComponent;
	class EnemyControllerComponent;
	class InputManager;
	class Scene;


	GameObject* AddPlayerGO(Scene& scene, InputManager& input, unsigned playerIndex, bool useKeyboard, const std::string& playerTexture);

	void AddEnemyTexture(std::shared_ptr<GameObject> enemy, const std::string& texturePath);

	dae::GameObject* AddEnemyGO(dae::Scene& scene, std::vector<TransformComponent*>& pPlayer, glm::vec3 pos,
		dae::EnemyTypes enemyType, std::vector<EnemyControllerComponent*>& butterflies);

	dae::GameObject* AddPlayerEnemyGO(dae::Scene& scene, std::vector<TransformComponent*>& pPlayer, glm::vec3 pos,
		 std::vector<EnemyControllerComponent*>& butterflies);

	void LoadUI(dae::Scene& scene, const std::string& nextScene, bool useSecondPlayer = false);
}