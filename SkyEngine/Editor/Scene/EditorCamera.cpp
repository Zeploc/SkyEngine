#include "EditorCamera.h"

#include "Editor/Windows/EditorWindowManager.h"

void EditorCamera::SpectatorUpdate()
{
	// When main window not focus don't add movement
	if (EditorWindowManager::GetCurrentFocus() != EditorWindowManager::MainWindow)
	{
		return;
	}
	
	CameraManager::SpectatorUpdate();
}
