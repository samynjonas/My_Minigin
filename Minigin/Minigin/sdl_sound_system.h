#pragma once
#include "sound_system.h"
#include <iostream>
#include <vector>
#include <string>

struct Mix_Chunk;

namespace dae
{
	class sdl_sound_system : public sound_system
	{
	public:
		sdl_sound_system();
		~sdl_sound_system();
		void play(const std::string& soundName) override;
		void AddSound(const std::string& name, int volume, const std::string& file_path) override;
		void Mute(bool state) override;
		bool GetMute() const override;

	private:
		class SDL_SOUND_SYSTEM_IMPL;
		std::unique_ptr<SDL_SOUND_SYSTEM_IMPL> pImp;

	};
}