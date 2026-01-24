// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once
#include "Core/Asset/AssetObject.h"

class ENGINE_API CAudio : public CAssetObject
{
public:
	CAudio();
	
	std::shared_ptr<CAudio> shared_from_this();
	
	std::string GetAssetClassName() override;
	static std::string GetStaticName(); 
	void Open() override;
	void OnLoaded() override;
	void OnUnloaded() override;

	std::string GetAbsolutePath() const;
	bool IsLooping() const { return bLoop; }
	
	std::string RelativeAudioPath;

protected:
	bool bLoop = false;
};
