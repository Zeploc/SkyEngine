#include "SEPCH.h"
#include "FmodAudio.h"

#include "Audio/Audio.h"
#include "Core/Asset/Asset.h"
#include "System/LogManager.h"

bool FmodAudio::Initialize()
{
	FMOD_RESULT Result = System_Create(&FmodSystem);
	if (Result != FMOD_OK)
	{
		LOG_WARNING("Failed to create FMOD audio manager");
		return false;
	}

	Result = FmodSystem->init(MaxChannels, FMOD_INIT_NORMAL, nullptr);
	if (Result != FMOD_OK)
	{
		LOG_WARNING("Failed to initialize FMOD audio manager");
		return false;
	}

	return true;
}

bool FmodAudio::RegisterAudioObject(TAssetObjectPointer<CAudio> NewAudio)
{
	if (!NewAudio)
	{
		return false;
	}
	FMOD::Sound* CreatedSound;
	FmodSystem->createSound(NewAudio->GetAbsolutePath().c_str(), FMOD_DEFAULT, nullptr, &CreatedSound);
	if (!CreatedSound)
	{
		LOG_WARNING("Failed to load sound from audio file {}", NewAudio->GetAbsolutePath());
		return false;
	}
	if (NewAudio->IsLooping())
	{
		CreatedSound->setMode(FMOD_LOOP_NORMAL);
	}
	const SoundObject NewSoundObj = {.AudioObject = NewAudio, .Sound = CreatedSound, .channel = nullptr};
	Sounds.push_back(NewSoundObj);
	LOG_MESSAGE("Loading audio file \"{}\": {}", NewAudio->Asset->DisplayName, NewAudio->RelativeAudioPath);
	return true;
}

bool FmodAudio::PlayAudio(TAssetObjectPointer<CAudio> AudioObject, bool bRestart)
{
	SoundObject* FoundObject = GetSoundObject(AudioObject);
	if (!FoundObject)
	{
		// Try to register if not found
		RegisterAudioObject(AudioObject);
		FoundObject = GetSoundObject(AudioObject);
		if (!FoundObject)
		{
			return false;
		}
	}
	FMOD::Channel* Channel = FoundObject->channel;
	if (Channel)
	{
		bool bIsPlaying = false;
		Channel->isPlaying(&bIsPlaying);

		// Try resume if channel is playing current audio
		FMOD::Sound* CurrentChannelSound = nullptr;
		Channel->getCurrentSound(&CurrentChannelSound);
		if (bIsPlaying && CurrentChannelSound && FoundObject->Sound == CurrentChannelSound)
		{
			if (bRestart)
			{
				Channel->setPosition(0, FMOD_TIMEUNIT_MS);
			}
			return Channel->setPaused(false) == FMOD_OK;
		}
	}
	FmodSystem->playSound(FoundObject->Sound, nullptr, false, &Channel);
	FoundObject->channel = Channel;
	return true;
}

bool FmodAudio::PauseAudio(TAssetObjectPointer<CAudio> AudioObject)
{
	SoundObject* FoundObject = GetSoundObject(AudioObject);
	if (!FoundObject)
	{
		return false;
	}
	FMOD::Channel* Channel = FoundObject->channel;
	if (!Channel)
	{
		return false;
	}
	FMOD::Sound* CurrentChannelSound;
	Channel->getCurrentSound(&CurrentChannelSound);
	if (!CurrentChannelSound || FoundObject->Sound != CurrentChannelSound)
	{
		// Channel wasn't currently playing this audio
		return false;
	}
	return Channel->setPaused(true) == FMOD_OK;
}

bool FmodAudio::StopAudio(TAssetObjectPointer<CAudio> AudioObject)
{
	SoundObject* FoundObject = GetSoundObject(AudioObject);
	if (!FoundObject)
	{
		return false;
	}
	FMOD::Channel* Channel = FoundObject->channel;
	if (!Channel)
	{
		return false;
	}
	FMOD::Sound* CurrentChannelSound;
	Channel->getCurrentSound(&CurrentChannelSound);
	if (!CurrentChannelSound || FoundObject->Sound != CurrentChannelSound)
	{
		// Channel wasn't currently playing this audio
		return false;
	}
	return Channel->stop() == FMOD_OK;
}

bool FmodAudio::SetAudioVolume(TAssetObjectPointer<CAudio> AudioObject, float VolumePercent)
{
	SoundObject* FoundObject = GetSoundObject(AudioObject);
	if (!FoundObject)
	{
		return false;
	}
	FMOD::Channel* Channel = FoundObject->channel;
	if (!Channel)
	{
		return false;
	}
	FMOD::Sound* CurrentChannelSound;
	Channel->getCurrentSound(&CurrentChannelSound);
	if (!CurrentChannelSound || FoundObject->Sound != CurrentChannelSound)
	{
		// Channel wasn't currently playing this audio
		return false;
	}
	return Channel->setVolume(VolumePercent);
}

bool FmodAudio::IsPlaying(TAssetObjectPointer<CAudio> AudioObject)
{
	SoundObject* FoundObject = GetSoundObject(AudioObject);
	if (!FoundObject)
	{
		return false;
	}
	FMOD::Channel* Channel = FoundObject->channel;
	if (!Channel)
	{
		return false;
	}
	FMOD::Sound* CurrentChannelSound;
	Channel->getCurrentSound(&CurrentChannelSound);
	if (!CurrentChannelSound || FoundObject->Sound != CurrentChannelSound)
	{
		// Channel wasn't currently playing this audio
		return false;
	}
	bool bIsPlaying;
	FMOD_RESULT Fmod_Result = Channel->isPlaying(&bIsPlaying);
	if (!bIsPlaying || Fmod_Result != FMOD_OK)
	{
		return false;
	}
	bool bIsPaused;
	Fmod_Result = Channel->getPaused(&bIsPaused);
	if (bIsPaused || Fmod_Result != FMOD_OK)
	{
		return false;
	}
	return true;
}

FmodAudio::SoundObject* FmodAudio::GetSoundObject(TAssetObjectPointer<CAudio> AudioObject)
{
	for (SoundObject& SndObj : Sounds)
	{
		if (SndObj.AudioObject == AudioObject)
		{
			return &SndObj;
		}
	}
	return nullptr;
}
