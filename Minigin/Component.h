#pragma once
#include "GameObject.h"

namespace dae
{
	class Component
	{

	public:
		Component() {};

		virtual ~Component() {};



		void Render() const {};
		void Update() {};

	private:


		std::shared_ptr<GameObject> m_Owner;
	};

}

