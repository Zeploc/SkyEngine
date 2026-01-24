// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Audio/AudioInterface.h"

#include <fmod/fmod.hpp>

class FmodAudio : public IAudioInterface
{
public:
	bool Initialize() override;

	bool RegisterAudioObject(TAssetObjectPointer<CAudio> NewAudio) override;
	bool PlayAudio(TAssetObjectPointer<CAudio> AudioObject, bool bRestart) override;
	bool PauseAudio(TAssetObjectPointer<CAudio> AudioObject) override;
	bool StopAudio(TAssetObjectPointer<CAudio> AudioObject) override;
	bool SetAudioVolume(TAssetObjectPointer<CAudio> AudioObject, float VolumePercent) override;
	bool IsPlaying(TAssetObjectPointer<CAudio> AudioObject) override;
	
private:	
	FMOD::System* FmodSystem = nullptr;
	int MaxChannels = 50;	

	struct SoundObject
	{
		TAssetObjectPointer<CAudio> AudioObject;
		FMOD::Sound* Sound;
		FMOD::Channel* channel;
	};	

	std::vector<SoundObject> Sounds;

	SoundObject* GetSoundObject(TAssetObjectPointer<CAudio> AudioObject);
};
