#include "Component.h"

dae::Component::Component()
{

}

dae::Component::~Component()
{

}

void dae::Component::SetOwner(GameObject* pOwner)
{
	m_pOwner = pOwner;
}