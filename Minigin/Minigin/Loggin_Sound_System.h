#pragma once
#include "sound_system.h"
#include <memory>

namespace dae
{
	class Loggin_Sound_System final : public sound_system
	{
	public:
		Loggin_Sound_System(std::unique_ptr<sound_system> ss);
		void play(const int id, const float& volume) override;
		void AddSound(const int id, const std::string& file_path) override;


	private:
		std::unique_ptr<sound_system> m_SoundSystem;

	};
}


