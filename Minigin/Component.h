#pragma once
#include <memory>

#include "GameObject.h"
namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;


		virtual void Render() const {};
		virtual void Update() {};

		virtual void RenderImGui() {};
	protected:

		explicit Component(std::weak_ptr<GameObject> owner);
		std::weak_ptr<GameObject> GetOwner() const;

	private:
		std::weak_ptr<GameObject> m_Owner;
	};

}

