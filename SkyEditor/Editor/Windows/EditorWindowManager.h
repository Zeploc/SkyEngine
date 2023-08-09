// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <vector>

#include "Core/Core.h"

class CEngineWindow;

class EditorWindowManager
{
public:
	EditorWindowManager();

	~EditorWindowManager();

	static void NewWindowCreated(class EditorWindow* InWindow);

	static void WindowRemoved(EditorWindow* _Window);

	static void UpdateWindows();

	static void RenderWindows();

	static void MainWindowSizeChanged(int _w, int _h);

	static void FocusChanged();

	static TPointer<CEngineWindow> GetCurrentFocus() { return CurrentFocused; };

	static std::vector<class EditorWindow*> GetWindows() { return EditorWindows; };

	static TPointer<CEngineWindow> GetMainWindow();	

protected:
	static bool IsRemovedID(EditorWindow* _Window);

public:

protected:
	// TODO: Link focus
	static TPointer<CEngineWindow> CurrentFocused;

	static std::vector<EditorWindow*> EditorWindows;
	static std::vector<EditorWindow*> EditorWindowsToRemove;
};
