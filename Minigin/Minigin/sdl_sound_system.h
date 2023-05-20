#pragma once
#include "sound_system.h"
#include <iostream>
#include <vector>

#include <map>

struct Mix_Chunk;

namespace dae
{
	class sdl_sound_system : public sound_system
	{
	public:
		sdl_sound_system();
		~sdl_sound_system();
		void play(const int id, const float& volume) override;
		void AddSound(const int id, const std::string& file_path) override;


	private:
		class SDL_SOUND_SYSTEM_IMPL;
		std::unique_ptr<SDL_SOUND_SYSTEM_IMPL> pImp;

	};
}