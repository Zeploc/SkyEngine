#include "EditorWindowManager.h"

#include "EditorWindow.h"

#include <freeglut.h>
#include <iostream>


std::map<int, EditorWindow*> EditorWindowManager::EditorWindows;

EditorWindowManager::EditorWindowManager()
{
}


EditorWindowManager::~EditorWindowManager()
{
}

EditorWindow* EditorWindowManager::GetCurrentWindow()
{
	HWND ActiveWindow = GetActiveWindow();
	char ActiveWindowName[101];
	GetWindowText(ActiveWindow, ActiveWindowName, 100);

	std::string CurrentWindow = "Main Window";
	for (auto it : EditorWindows)
	{
		EditorWindow* CurrentWindow = it.second;
		if (CurrentWindow)
		{
			std::string CurrentName = CurrentWindow->GetWindowName();
			if (CurrentName == ActiveWindowName)
			{
				std::cout << CurrentName.c_str() << " Window Active" << std::endl;
				return CurrentWindow;
			}
		}
	}

	std::cout << "Main Window Active " << std::endl;
}

void EditorWindowManager::StaticRenderWindow()
{
	int CurrentWindow = glutGetWindow();

	// BAD ??
	if (EditorWindows.find(CurrentWindow)._Ptr)
	{
		EditorWindow* FoundWindow = EditorWindows.find(CurrentWindow)->second;
		if (FoundWindow)
		{
			FoundWindow->RenderWindow();
		}
	}
}

void EditorWindowManager::NewWindowCreated(EditorWindow * _window)
{
	EditorWindows.insert(std::pair<int, EditorWindow*>(_window->GetWindowID(), _window));
}

void EditorWindowManager::UpdateWindows()
{
	for (auto it : EditorWindows)
	{
		EditorWindow* CurrentWindow = it.second;
		if (CurrentWindow)
			CurrentWindow->UpdateWindow();
	}
}

void EditorWindowManager::MainWindowSizeChanged(int _w, int _h)
{
	for (auto it : EditorWindows)
	{
		EditorWindow* CurrentWindow = it.second;
		if (CurrentWindow)
			CurrentWindow->MainWindowSizeChanged(_w, _h);
	}
}
