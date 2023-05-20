#include "Commands.h"
#include <iostream>
#include "GameObject.h"

#include "GunComponent.h"
#include "servicelocator.h"
#include "sound_system.h"

dae::ShootCommand::ShootCommand(GameObject* pGameObject)
	: Command(pGameObject)
{
	m_pGunComponent = pGameObject->GetComponent<GunComponent>();
}

void dae::ShootCommand::Execute()
{
	if (m_pGunComponent != nullptr)
	{
		m_pGunComponent->Fire();
	}
}

dae::TestCommand_playSound::TestCommand_playSound(GameObject* pGameObject)
	: Command(pGameObject)
{

}

void dae::TestCommand_playSound::Execute()
{
	auto& ss = servicelocator<sound_system>::get_serviceLocator();
	ss.play(0, 100);
}