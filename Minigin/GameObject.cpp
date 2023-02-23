#include <string>
#include <type_traits>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"



void dae::GameObject::Update()
{

	for (auto& component : m_Components)
	{
		component->Update();
	}
}
