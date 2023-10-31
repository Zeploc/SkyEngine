// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Core/Core.h"
#include "Math/Vector2.h"

class IFramebuffer;
class Scene;

class ENGINE_API CSceneRenderer
{
public:
	void Init(TPointer<Scene> InTargetScene, SVector2i InSize);
	void SizeChanged(SVector2i InSize);
	void Render();
	void SetSceneTarget(TPointer<Scene> InTargetScene);

	TPointer<IFramebuffer> GetFramebuffer() const { return Framebuffer; }
	
	// TODO: Create colour type
	SVector ClearColour = SVector(0.3f, 0.8f, 0.9f);
	
protected:
	// TODO: Multiple frame buffers (multiple windows, etc)
	TPointer<IFramebuffer> Framebuffer;

	TPointer<Scene> TargetScene;
	SVector2i ViewSize;

	// TODO: Enabled/disabled?
};
