// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "FmodAudio.h"
#include "Platform/PlatformInterface.h"

class WindowsPlatform : public IPlatformInterface
{
public:
	~WindowsPlatform() override;
	std::string GetPlatformDisplayName() override;
	
	TSharedPointer<IAudioInterface> GetAudioInterface() override;
	TSharedPointer<CEngineWindow> CreateNewWindow(const std::string& InWindowName, SVector2i InWindowSize, bool bFullScreen) override;
	double GetTime() override;
	int32_t DisplayMessageBox(std::string Title, std::string Message, uint32_t Options) override;

private:
	std::shared_ptr<FmodAudio> Audio;
};
