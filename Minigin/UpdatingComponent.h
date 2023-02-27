#pragma once
#include "Component.h"

namespace dae {


	class UpdatingComponent :
		public Component
	{

	public:
		
		UpdatingComponent(std::weak_ptr<GameObject> owner);

		virtual ~UpdatingComponent() = default;
		UpdatingComponent(const UpdatingComponent& other) = delete;
		UpdatingComponent(UpdatingComponent&& other) = delete;
		UpdatingComponent& operator=(const UpdatingComponent& other) = delete;
		UpdatingComponent& operator=(UpdatingComponent&& other) = delete;

		virtual void Update() {};


	private:
	};

}

