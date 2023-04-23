// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Engine/Camera/CameraManager.h"

class EditorCamera : public CameraManager
{
public:

protected:
	void SpectatorUpdate() override;
	
};
