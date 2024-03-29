#pragma once
#include <memory>
#include <vector>
#include <stdexcept>
//#include "UpdatingComponent.h" //when uncomment issues

namespace dae
{
	class Component;
	class TransformComponent;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:

		GameObject(int depthValue = 0);
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template <typename T> std::shared_ptr <T> AddComponent();
		template <typename T> std::shared_ptr <T> GetComponent();
		template <typename T> void RemoveComponent();

		void Init();
		void Update();
		void Render() const;

		void SetParent(GameObject* parent, bool keepWorldPosition);

		std::shared_ptr<dae::TransformComponent> GetTransform() const;

		const GameObject* GetParent() const;

		void ClearChildren();

		std::vector<std::shared_ptr<GameObject>> GetChildren() const;

		bool IsDestroyed();

		int GetDrawDepth();

		void MarkForDestroy();

	private:

		void RemoveChild(const std::shared_ptr<GameObject>& child);
		void AddChild(const std::shared_ptr<GameObject>& child);

		//TODO: convert into raw pointer
		GameObject* m_Parent{ nullptr };

		std::vector<std::shared_ptr<GameObject>> m_Children;

		std::vector<std::shared_ptr<Component>> m_Components{};

		//All gameobjects have a transform
		std::shared_ptr<TransformComponent> m_Transform{};


		bool m_IsDestroyed{ false };

		int m_DepthValue{ 0 };
	};



	template<typename T>
	inline std::shared_ptr <T> dae::GameObject::AddComponent()
	{
		std::shared_ptr <T> foundComponent = GetComponent<T>();
		if (foundComponent)
		{
			return foundComponent;
		}


		static_assert(std::is_base_of<Component, T>::value && "T must inherit from Component");

		auto component = std::make_shared<T>(this);


		m_Components.emplace_back(component);

		return component;
	}

	template<typename T>
	inline std::shared_ptr <T> dae::GameObject::GetComponent()
	{
		for (auto& component : m_Components)
		{
			std::shared_ptr<T> castComponent = std::dynamic_pointer_cast<T>(component);
			if (castComponent)
				return castComponent;
		}

		return nullptr;
	}

	template<typename T>
	inline void dae::GameObject::RemoveComponent()
	{

		if (typeid(T).name() == typeid(TransformComponent).name())
		{
			throw std::invalid_argument("Cannot remove the Transform from a gameobject");
		}

		auto remove = [&](const auto& component) {
			auto castComponent = std::dynamic_pointer_cast<T>(component);

			return castComponent;
		};

		m_Components.erase(
			std::remove_if
			(
				begin(m_Components),
				end(m_Components),
				remove
			),
			end(m_Components));

	}

}

