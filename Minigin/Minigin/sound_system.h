#pragma once
#include <string>

namespace dae
{
	class sound_system
	{
	public:
		virtual ~sound_system() = default;
		virtual void play(const std::string& soundName) = 0;
		virtual void AddSound(const std::string& name, int volume, const std::string& file_path) = 0;

	};

	class NULL_sound_system final : public sound_system
	{
	public:
		~NULL_sound_system() = default;
		void play(const std::string& /*soundName*/) override {};
		void AddSound(const std::string& /*name*/, int /*volume*/, const std::string& /*file_path*/) override {};

	};
}



