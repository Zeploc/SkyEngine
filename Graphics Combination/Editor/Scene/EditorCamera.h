// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Engine/Camera/Camera.h"

class EditorCamera : public Camera
{
public:

protected:
	void SpectatorControls() override;
	
};
