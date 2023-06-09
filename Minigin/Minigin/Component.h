#pragma once

namespace dae
{
	class GameObject;
	class Component
	{
	public:
		Component();
		virtual ~Component();

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Update() = 0;
		virtual void LateUpdate() {};
		void SetOwner(GameObject* pOwner);

	protected:
		GameObject* GetOwner() const
		{
			return m_pOwner;
		}


	private:
		GameObject* m_pOwner{ nullptr };
	};
}



