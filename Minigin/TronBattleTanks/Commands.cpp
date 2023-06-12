#include "Commands.h"
#include <iostream>
#include "GameObject.h"

#include "GunComponent.h"
#include "servicelocator.h"
#include "sound_system.h"

//----------------------- Shoot Command --------------------------------------------------

dae::ShootCommand::ShootCommand(GameObject* pGameObject)
	: Command(pGameObject)
{
	m_pGunComponent = pGameObject->GetComponent<GunComponent>();
}

void dae::ShootCommand::Execute()
{
	if (m_pGunComponent != nullptr)
	{
		//Firing gun
		m_pGunComponent->Fire();
	}
}


//----------------------- TestCommand playSound ------------------------------------------

dae::TestCommand_playSound::TestCommand_playSound(GameObject* pGameObject)
	: Command(pGameObject)
{

}

void dae::TestCommand_playSound::Execute()
{
	auto& ss = servicelocator<sound_system>::get_serviceLocator();
	ss.play("Shoot");
}

//----------------------- Letter up command ------------------------------------------
dae::LetterUpCommand::LetterUpCommand(GameObject* pGameobject)
	: Command(pGameobject)
{

}

void dae::LetterUpCommand::Execute()
{

}

//----------------------- Letter up command ------------------------------------------
dae::LetterDownCommand::LetterDownCommand(GameObject* pGameobject)
	: Command(pGameobject)
{

}

void dae::LetterDownCommand::Execute()
{

}