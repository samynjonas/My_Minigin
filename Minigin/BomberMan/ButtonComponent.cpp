#include "ButtonComponent.h"

dae::ButtonComponent::ButtonComponent()
{

}

void dae::ButtonComponent::Update()
{

}

void dae::ButtonComponent::Select()
{
	//Change Texture
	m_IsSelected = true;
}

void dae::ButtonComponent::DeSelect()
{
	//Change Texture
	m_IsSelected = false;
}

void dae::ButtonComponent::Press()
{
	if (m_IsSelected == false)
	{
		return;
	}

	NotifyObservers(ButtonPressed, this);
}