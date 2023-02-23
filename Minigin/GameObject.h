#pragma once
#include <memory>
#include <vector>
#include "Component.h"

namespace dae
{
	class Component;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:

		GameObject() = default;

		template <typename T> T* AddComponent();
		template <typename T> T* GetComponent();
		template <typename T> void RemoveComponent();

		void Update();

	private:
		std::vector<std::shared_ptr<Component>> m_Components{};
	};



	template<typename T>
	inline T* dae::GameObject::AddComponent()
	{
		T* foundComponent = GetComponent<T>();
		if (foundComponent)
		{
			return foundComponent;
		}

		static_assert(std::is_base_of<Component, T>::value && "T must inherit from Compnent");

		auto component = std::make_shared<T>();

		component->SetOwner(shared_from_this());

		m_Components.push_back(component);


		return component.get();
	}

	template<typename T>
	inline T* dae::GameObject::GetComponent()
	{
		for (auto& component : m_Components)
		{
			std::shared_ptr<T> castComponent = std::dynamic_pointer_cast<T>(component);
			if (castComponent)
				return castComponent.get();
		}

		return nullptr;
	}

	template<typename T>
	inline void dae::GameObject::RemoveComponent()
	{
		auto remove = [&](const auto& component) { return std::is_same<T, decltype(component)>::value; };
		m_Components.erase(
			std::remove_if
			(
				begin(m_Components),
				end(m_Components),
				remove,
				end(m_Components)
			));
	}

}

