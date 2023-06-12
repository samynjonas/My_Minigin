#include "Loggin_Sound_System.h"
#include <iostream>
#include "servicelocator.h"
#include "Logger.h"

dae::Loggin_Sound_System::Loggin_Sound_System(std::unique_ptr<sound_system> ss)
	: m_SoundSystem{ std::move(ss) }
{

}

void dae::Loggin_Sound_System::play(const std::string& soundName)
{
	servicelocator<dae::Logger>::get_serviceLocator().Log({ "Playing: " + soundName });

	//std::cout << "playing " << id << " on " << volume << "%" << std::endl;
	m_SoundSystem->play(soundName);
}

void dae::Loggin_Sound_System::AddSound(const std::string& name, int volume, const std::string& file_path)
{
	servicelocator<dae::Logger>::get_serviceLocator().Log({ "Added: " + name + " to play at volume " + std::to_string(volume) });

	m_SoundSystem->AddSound(name, volume, file_path);

}

void dae::Loggin_Sound_System::Mute(bool state)
{
	servicelocator<dae::Logger>::get_serviceLocator().Log({ "Set music to mute = " + std::to_string(state) });
	m_SoundSystem->Mute(state);
}


bool dae::Loggin_Sound_System::GetMute() const
{
	bool muteStatus = m_SoundSystem->GetMute();

	servicelocator<dae::Logger>::get_serviceLocator().Log({ "Is sound currently muted = " + std::to_string(muteStatus) });

	return muteStatus;
}