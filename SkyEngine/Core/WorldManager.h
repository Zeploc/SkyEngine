// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include "Math/Internal/Vector2.decl.h"

class Scene;

class CWorldManager
{
public:
	void SwitchScene(TPointer<Scene> InTargetScene);
	
	bool OnMouseButtonPressed(int MouseButton, int Mods);
	bool OnMouseButtonReleased(int MouseButton, int Mods);
	bool OnMouseMoved(SVector2i MousePos);
	bool OnMouseScrolled(float XOffset, float YOffset);
	bool OnKeyPressed(int KeyCode, int Mods, int RepeatCount);
	bool OnKeyTyped(int KeyCode, int Mods);
	bool OnKeyReleased(int KeyCode, int Mods);

protected:
	TPointer<Scene> TargetScene;

	// TODO: Current view camera
};
