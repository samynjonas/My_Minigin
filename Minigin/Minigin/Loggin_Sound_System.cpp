#include "Loggin_Sound_System.h"
#include <iostream>
#include "servicelocator.h"
#include "Logger.h"

dae::Loggin_Sound_System::Loggin_Sound_System(std::unique_ptr<sound_system> ss)
	: m_SoundSystem{ std::move(ss) }
{

}

void dae::Loggin_Sound_System::play(const int id, const float& volume)
{
	std::string logText = "Playing: " + id;

	servicelocator<dae::Logger>::get_serviceLocator().Log(logText);

	//std::cout << "playing " << id << " on " << volume << "%" << std::endl;
	m_SoundSystem->play(id, volume);
}

void dae::Loggin_Sound_System::AddSound(const int id, const std::string& file_path)
{
	std::string logText = "Add: " + file_path;

	servicelocator<dae::Logger>::get_serviceLocator().Log(logText);

	m_SoundSystem->AddSound(id, file_path);

}