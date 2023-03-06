#include <string>
#include <type_traits>
#include "GameObject.h"
#include "ResourceManager.h"
#include "TransformComponent.h"




void dae::GameObject::Init()
{
	m_Transform = AddComponent<TransformComponent>();
}

void dae::GameObject::Update()
{
	for (auto& component : m_Components)
	{
		component->Update();
	}
}

void dae::GameObject::Render() const
{
	for (auto& component : m_Components)
	{
		component->Render();
	}

}

void dae::GameObject::SetParent(std::shared_ptr<GameObject> parent, bool keepWorldPosition)
{
	auto transformLocked = m_Transform;
	if (!parent)
		transformLocked->SetLocalPosition(transformLocked->GetWorldPosition());
	else
	{
		if (keepWorldPosition)
		{
			auto parentTransformLocked = parent->m_Transform;
			transformLocked->SetLocalPosition(transformLocked->GetLocalPosition() - parentTransformLocked->GetWorldPosition());
		}
		transformLocked->SetPositionDirty();
	}

	if (!m_Parent.expired())
		m_Parent.lock()->RemoveChild(shared_from_this());
	m_Parent = parent;
	if (!m_Parent.expired())
		m_Parent.lock()->AddChild(shared_from_this());
}

std::shared_ptr<dae::TransformComponent> dae::GameObject::GetTransform()
{
	return m_Transform;
}

const std::weak_ptr<dae::GameObject>& dae::GameObject::GetParent() const
{
	return m_Parent;
}


void dae::GameObject::RemoveChild(const std::shared_ptr<GameObject>& child)
{
	m_Children.erase(std::remove(begin(m_Children), end(m_Children), child));
}

void dae::GameObject::AddChild(const std::shared_ptr<GameObject>& child)
{
	m_Children.push_back(child);
}