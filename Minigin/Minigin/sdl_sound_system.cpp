#include "sdl_sound_system.h"

#include "SDL.h"
#include "SDL_mixer.h"

#include <memory>

#include "Logger.h"
#include "servicelocator.h"

using namespace dae;
class sdl_sound_system::SDL_SOUND_SYSTEM_IMPL
{
public:
	SDL_SOUND_SYSTEM_IMPL()
	{
		if (Mix_Init(MIX_INIT_MP3) != 0)
		{
			servicelocator<Logger>::get_serviceLocator().LogWarning("SLD_MIXER Initialization Succesfull");
		}
		else
		{
			servicelocator<Logger>::get_serviceLocator().LogError("SLD_MIXER Initialization failed");
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) == 0)
		{
			servicelocator<Logger>::get_serviceLocator().LogWarning("SLD_Audio_device opened successfully");
		}
		else
		{
			servicelocator<Logger>::get_serviceLocator().LogError("Failed to open SDL_audio_device");
		}

		if (Mix_AllocateChannels(4))
		{
			servicelocator<Logger>::get_serviceLocator().LogWarning("Allocated mixing channels");
		}
		else
		{
			servicelocator<Logger>::get_serviceLocator().LogError("Unable to allocate mixing channels");
		}
	}
	~SDL_SOUND_SYSTEM_IMPL()
	{
		for (auto& sound : m_MusicSamples)
		{
			Mix_FreeChunk(sound.second);
		}
		Mix_CloseAudio();
		Mix_Quit();
	}

	void AddSound(const int id, const std::string& file_path)
	{
		if (m_MusicSamples[id] != nullptr)
		{
			servicelocator<Logger>::get_serviceLocator().LogWarning("Id was already used, override of this id has been done");
		}

		m_WAVFilenames.push_back(file_path);

		Mix_Chunk* sample = Mix_LoadWAV(file_path.c_str());
		if (sample == nullptr)
		{
			servicelocator<Logger>::get_serviceLocator().LogError("Could not add audio track");
			return;
		}

		m_MusicSamples[id] = sample;
	}
	void play(const int id, const float& volume)
	{
		if (m_MusicSamples[id] == nullptr)
		{
			servicelocator<Logger>::get_serviceLocator().LogError("No Audio track found");
			return;
		}

		Mix_VolumeChunk(m_MusicSamples[id], static_cast<int>(volume));
		Mix_PlayChannel(-1, m_MusicSamples[id], 0);
	}

private:
	std::vector<std::string> m_WAVFilenames;
	std::map<int, Mix_Chunk*> m_MusicSamples;

};



sdl_sound_system::sdl_sound_system()
	: sound_system()
{
	pImp = std::make_unique<SDL_SOUND_SYSTEM_IMPL>();
}

sdl_sound_system::~sdl_sound_system()
{
	
}

void sdl_sound_system::AddSound(const int id, const std::string& file_path)
{
	pImp->AddSound(id, file_path);
}

void sdl_sound_system::play(const int id, const float& volume)
{
	pImp->play(id, volume);
}