#include "SEPCH.h"
#include "WorldManager.h"

#include "Application.h"
#include "Scene/Scene.h"
#include "System/LogManager.h"

void CWorldManager::SwitchScene(TPointer<Scene> InTargetScene)
{
	TargetScene = InTargetScene;
}

// TODO: Switch to passing to WorldInput which the scene retreives - What about multiple input sources? (multiple viewports viewing same scene)
bool CWorldManager::OnMouseButtonPressed(int MouseButton, int Mods)
{
	if (!TargetScene)
	{
		return false;
	}
	return TargetScene->OnMouseButtonPressed(MouseButton, Mods);
}

bool CWorldManager::OnMouseButtonReleased(int MouseButton, int Mods)
{
	if (!TargetScene)
	{
		return false;
	}
	return TargetScene->OnMouseButtonReleased(MouseButton, Mods);
}

bool CWorldManager::OnMouseMoved(SVector2i MousePos)
{
	if (!TargetScene)
	{
		return false;
	}
	return TargetScene->OnMouseMoved(MousePos);
}

bool CWorldManager::OnMouseScrolled(float XOffset, float YOffset)
{
	if (!TargetScene)
	{
		return false;
	}
	return TargetScene->OnMouseScrolled(XOffset, YOffset);
}

bool CWorldManager::OnKeyPressed(int KeyCode, int Mods, int RepeatCount)
{
	if (!TargetScene)
	{
		return false;
	}
	return TargetScene->OnKeyPressed(KeyCode, Mods, RepeatCount);
}

bool CWorldManager::OnKeyTyped(int KeyCode, int Mods)
{
	if (!TargetScene)
	{
		return false;
	}
	// return TargetScene->OnKeyTyped(KeyCode, Mods);
	return false;
}

bool CWorldManager::OnKeyReleased(int KeyCode, int Mods)
{
	if (!TargetScene)
	{
		return false;
	}
	return TargetScene->OnKeyReleased(KeyCode, Mods);
}