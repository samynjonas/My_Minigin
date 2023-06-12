#pragma once
#include "sound_system.h"
#include <memory>

namespace dae
{
	class Loggin_Sound_System final : public sound_system
	{
	public:
		Loggin_Sound_System(std::unique_ptr<sound_system> ss);
		void play(const std::string& soundName) override;
		void AddSound(const std::string& name, int volume, const std::string& file_path) override;
		void Mute(bool state) override;
		bool GetMute() const override;

	private:
		std::unique_ptr<sound_system> m_SoundSystem;

	};
}


