// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Object.h"

class CAudio;

/*
 * Control active audio playing via audio objects
 */
class IAudioInterface
{
public:
	virtual ~IAudioInterface() = default;
	virtual bool Initialize() = 0;

	/**
	 * Adds an audio file to the audio system
	 * @param NewAudio Audio object to register
	 * @return true if added, false if failed or already exists
	 */
	virtual bool RegisterAudioObject(TAssetObjectPointer<CAudio> NewAudio) = 0;

	/**
	 * Play the given audio, optionally restarting otherwise will resume if paused
	 * @param AudioObject Audio Object to play
	 * @param bRestart Restart audio even if it was midway
	 * @return True if played, false if failed to play
	 */
	virtual bool PlayAudio(TAssetObjectPointer<CAudio> AudioObject, bool bRestart) = 0;

	/**
	 * Pause the given audio object if it's playing
	 * @param AudioObject Audio Object to pause
	 * @return True if stopped, false if audio wasn't currently playing or failed to pause
	 */
	virtual bool PauseAudio(TAssetObjectPointer<CAudio> AudioObject) = 0;

	/**
	 * Stop the given audio object if it's playing
	 * @param AudioObject Audio Object to stop
	 * @return True if stopped, false if audio wasn't currently playing or failed to stop
	 */
	virtual bool StopAudio(TAssetObjectPointer<CAudio> AudioObject) = 0;

	/**
	 * Set the current volume of existing playing audio
	 * @param AudioObject Audio Object to alter volume
	 * @param VolumePercent New volume between 0 and 1
	 * @return If audio was playing and successfully set volume
	 */
	virtual bool SetAudioVolume(TAssetObjectPointer<CAudio> AudioObject, float VolumePercent) = 0;

	/**
	 * Check whether an audio object is currently playing and not paused
	 * @param AudioObject Audio Object to check
	 * @return True if playing not paused, false if not, or an error occurred
	 */
	virtual bool IsPlaying(TAssetObjectPointer<CAudio> AudioObject) = 0;
};
