#include <string>
#include <type_traits>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "UpdatingComponent.h"
#include "RenderComponent.h"



void dae::GameObject::Update()
{

	for (auto& component : m_UpdatingComponents)
	{
		component->Update();
	}
}

void dae::GameObject::Render() const
{
	for (auto& component : m_RenderComponents)
	{
		component->Render();
	}
}

