#pragma once
#include <functional>
#include "Components/Component.h"

namespace dae
{

	class TextRendererComponent;
	class ButtonComponent : public Component
	{
	public:

		ButtonComponent(GameObject* owner);

		virtual ~ButtonComponent() = default;
		ButtonComponent(const ButtonComponent& other) = delete;
		ButtonComponent(ButtonComponent&& other) = delete;
		ButtonComponent& operator=(const ButtonComponent& other) = delete;
		ButtonComponent& operator=(ButtonComponent&& other) = delete;


		void SetActive(bool isActive);

		void DoClick();


		void SetButtonText(const std::string& text);

		void SetOnClick(std::function<void()> onClick);
	private:

		TextRendererComponent* m_ActiveRender{};
		TextRendererComponent* m_InactiveRender{};

		std::function<void()> m_OnClick {};
	};
}

