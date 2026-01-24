#include "SEPCH.h"
#include "Audio.h"

#include "AudioInterface.h"
#include "Core/Application.h"
#include "Platform/PlatformInterface.h"
#include "System/LogManager.h"

CAudio::CAudio()
{
	SetSerializeVariable(RelativeAudioPath);
	SetSerializeVariable(bLoop);
}

std::shared_ptr<CAudio> CAudio::shared_from_this()
{
	return std::static_pointer_cast<CAudio>(CAssetObject::shared_from_this());
}

std::string CAudio::GetAssetClassName()
{
	return GetStaticName();
}

std::string CAudio::GetStaticName()
{
	return "Audio";
}

void CAudio::Open()
{
	// For now play and pause audio
	TSharedPointer<IAudioInterface> AudioInterface = GetPlatformInterface()->GetAudioInterface();
	if (AudioInterface->IsPlaying(shared_from_this()))
	{
		AudioInterface->PauseAudio(shared_from_this());
	}
	else
	{
		AudioInterface->PlayAudio(shared_from_this(), false);
	}
}

void CAudio::OnLoaded()
{
	GetPlatformInterface()->GetAudioInterface()->RegisterAudioObject(shared_from_this());
}

void CAudio::OnUnloaded()
{
	CAssetObject::OnUnloaded();
}

std::string CAudio::GetAbsolutePath() const
{
	return GetApplication()->GetContentDirectory() + RelativeAudioPath;
}
