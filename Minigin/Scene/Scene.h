#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class CollisionComponent;
	struct Event;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Update();
		void Render() const;


		void AddCollision(CollisionComponent* collision);
		void RemoveCollision(CollisionComponent* collision);


		const std::string& GetName() const;


		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		bool IsActive() { return m_IsActive; };
		void SetActive(bool isActive) { m_IsActive = isActive; };

	private:
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector <std::shared_ptr<GameObject>> m_Objects{};
		std::vector <CollisionComponent*> m_ObjectCollisions{};


		bool m_IsActive{ false };
		void SortGameObjectUpdate();
		void NormalUpdate();
		void CleanUpdate();


		void HandleDestroyedEvent(const Event* e);


		static unsigned int m_IdCounter;

		bool m_WasGameObjectAdded{ false };
		bool m_WasObjectDestroyed{ false };
	};

}
