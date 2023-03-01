#pragma once
#include <memory>
#include <vector>
//#include "UpdatingComponent.h" when uncomment issues

namespace dae
{
	class Component;
	class UpdatingComponent;
	class RenderComponent;

	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:

		GameObject() = default;
		~GameObject() = default;
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template <typename T> std::shared_ptr <T> AddComponent();
		template <typename T> std::shared_ptr <T> GetComponent();
		template <typename T> void RemoveComponent();

		void Update();
		void Render() const;

	private:
		std::vector<std::shared_ptr<Component>> m_Components{};
		std::vector<std::shared_ptr<UpdatingComponent>> m_UpdatingComponents{};
		std::vector<std::shared_ptr<RenderComponent>> m_RenderComponents{};
	};



	template<typename T>
	inline std::shared_ptr <T> dae::GameObject::AddComponent()
	{
		std::shared_ptr <T> foundComponent = GetComponent<T>();
		if (foundComponent)
		{
			return foundComponent;
		}


		static_assert(std::is_base_of<Component, T>::value && "T must inherit from Compnent");

		auto component = std::make_shared<T>(shared_from_this());


		m_Components.emplace_back(component);

		auto updatingCast = std::dynamic_pointer_cast<UpdatingComponent>(component);
		if (updatingCast)
		{
			m_UpdatingComponents.emplace_back(updatingCast);
		}

		auto renderCast = std::dynamic_pointer_cast<RenderComponent>(component);
		if (renderCast)
		{
			m_RenderComponents.emplace_back(renderCast);
		}

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



		m_UpdatingComponents.erase(
			std::remove_if
			(
				begin(m_UpdatingComponents),
				end(m_UpdatingComponents),
				remove
			),
			end(m_UpdatingComponents));

		m_RenderComponents.erase(
			std::remove_if
			(
				begin(m_RenderComponents),
				end(m_RenderComponents),
				remove
			),
			end(m_RenderComponents));

	}

}

