#pragma once
#include <string>

namespace dae
{
	class sound_system
	{
	public:
		virtual ~sound_system() = default;
		virtual void play(const int id, const float& volume) = 0;
		virtual void AddSound(const int id, const std::string& file_path) = 0;

	};
}



