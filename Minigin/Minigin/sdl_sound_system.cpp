#include "sdl_sound_system.h"

#include "SDL.h"
#include "SDL_mixer.h"

#include <memory>
#include <thread>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <queue>


#include "Logger.h"
#include "servicelocator.h"

using namespace dae;
class sdl_sound_system::SDL_SOUND_SYSTEM_IMPL final : public sound_system
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

		m_Thread = std::jthread{ [&]() { ThreadUpdateLoop(); } };
	}
	~SDL_SOUND_SYSTEM_IMPL()
	{
		m_IsMarkedDestroy = true;
		m_ThreadCondition.notify_one();

		for (auto& sound : m_MusicSamples)
		{
			Mix_FreeChunk(sound.pSound);
		}
		Mix_CloseAudio();
		Mix_Quit();

		m_MusicSamples.clear();
		m_WAVFilenames.clear();

		m_Thread.join();
	}

	struct SoundData
	{
		Mix_Chunk* pSound{ nullptr };
		int volume{ 128 };
	};

	void AddSound(const std::string& name, int volume, const std::string& file_path) override
	{
		int soundID = StringToID(name);

		if (soundID != -1)
		{
			servicelocator<Logger>::get_serviceLocator().LogError({ name + " was already used for a sound" });
			return;
		}

		Mix_Chunk* sample = Mix_LoadWAV(file_path.c_str());
		if (sample == nullptr)
		{
			servicelocator<Logger>::get_serviceLocator().LogError("Could not add audio track");
			return;
		}

		m_WAVFilenames.push_back(name);
		m_MusicSamples.push_back({ sample, volume });
	}	
	void play(const std::string& soundName) override
	{
		int soundID = StringToID(soundName);
		if (soundID == -1)
		{
			servicelocator<Logger>::get_serviceLocator().LogError({ "No Audio track found with name " + soundName});
			return;
		}

		if (m_MusicSamples[soundID].pSound == nullptr)
		{
			servicelocator<Logger>::get_serviceLocator().LogError("No Audio track attached to " + soundName);
			return;
		}

		m_AudioQueue.push(soundID);
		m_ThreadCondition.notify_one();
	}

	int StringToID(const std::string& audio)
	{
		for (int i = 0; i < m_WAVFilenames.size(); i++)
		{
			if (m_WAVFilenames[i] == audio)
			{
				return i;
			}
		}
		return -1;
	}

	void Mute(bool state) override
	{
		m_IsNotMuted = !state;
	}

	void ThreadUpdateLoop()
	{
		//Will handle the request
		while (m_IsMarkedDestroy == false)
		{
			std::unique_lock lock(m_Mutex);
			m_ThreadCondition.wait(lock, [&] { return (m_AudioQueue.size() > 0) || m_IsMarkedDestroy; });

			while (m_AudioQueue.size() > 0)
			{
				int audioRequest{ m_AudioQueue.front() };
				lock.unlock();

				if (audioRequest < 0 || audioRequest >= m_MusicSamples.size())
				{
					//Error
					return;
				}
				SoundData sound = m_MusicSamples[audioRequest];
				if (sound.pSound == nullptr)
				{
					return;
				}

				Mix_VolumeChunk(sound.pSound, sound.volume * m_IsNotMuted);
				Mix_PlayChannel(-1, sound.pSound, 0);

				lock.lock();
				m_AudioQueue.pop();
				lock.unlock();
			}
		}
	}

	bool GetMute() const override
	{
		return !m_IsNotMuted;
	}

private:
	std::vector<std::string> m_WAVFilenames;
	std::vector<SoundData> m_MusicSamples;
	std::queue<int> m_AudioQueue;

	std::jthread m_Thread;
	std::mutex m_Mutex{};
	std::condition_variable m_ThreadCondition{};

	std::atomic<bool> m_IsMarkedDestroy{ false };

	bool m_IsNotMuted{ true };
};



sdl_sound_system::sdl_sound_system()
	: sound_system()
{
	pImp = std::make_unique<SDL_SOUND_SYSTEM_IMPL>();
}

sdl_sound_system::~sdl_sound_system()
{
	
}

void sdl_sound_system::AddSound(const std::string& name, int volume, const std::string& file_path)
{
	pImp->AddSound(name, volume, file_path);
}

void sdl_sound_system::play(const std::string& soundName)
{
	pImp->play(soundName);
}

void sdl_sound_system::Mute(bool state)
{
	pImp->Mute(state);
}

bool sdl_sound_system::GetMute() const
{
	return pImp->GetMute();
}