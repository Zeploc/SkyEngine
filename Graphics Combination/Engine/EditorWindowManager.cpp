#include "EditorWindowManager.h"

#include "EditorWindow.h"
#include "Input.h"

#include <glew.h>
#include <GLFW/glfw3.h>
//#include <freeglut.h>
#include <iostream>


std::map<int, EditorWindow*> EditorWindowManager::EditorWindows;
std::vector<int> EditorWindowManager::EditorWindowsToRemove;
GLFWwindow* EditorWindowManager::MainWindow = nullptr;

EditorWindowManager::EditorWindowManager()
{

}


EditorWindowManager::~EditorWindowManager()
{
}

EditorWindow* EditorWindowManager::GetCurrentWindow()
{
	//HWND ActiveWindow = GetActiveWindow();
	//char ActiveWindowName[101];
	//GetWindowText(ActiveWindow, ActiveWindowName, 100);

	//// Check for sub windows
	//int CurrentWindowID = glutGetWindow();
	//auto Current = EditorWindows.find(CurrentWindowID);
	////auto theEND = ;
	//if (Current != EditorWindows.end())
	//{
	//	EditorWindow* CurrentWindow = Current->second;
	//	if (CurrentWindow)
	//	{
	//		std::string CurrentName = CurrentWindow->GetWindowName();
	//		//std::cout << CurrentName.c_str() << " Window Active" << std::endl;
	//		return CurrentWindow;
	//	}

	//}

	//// Check for external windows
	//for (auto it : EditorWindows)
	//{
	//	EditorWindow* CurrentWindow = it.second;
	//	if (CurrentWindow)
	//	{
	//		std::string CurrentName = CurrentWindow->GetWindowName();
	//		if (CurrentName == ActiveWindowName)
	//		{
	//			//std::cout << CurrentName.c_str() << " Window Active" << std::endl;
	//			return CurrentWindow;
	//		}
	//	}
	//}
	
	// No windows, so is main
	//std::cout << "Main Window Active " << std::endl;
	return nullptr;
}

bool EditorWindowManager::IsRemovedID(int _WindowID)
{
	for (int id : EditorWindowsToRemove)
	{
		if (id == _WindowID)
			return true;
	}
	return false;
}

void EditorWindowManager::StaticRenderWindow()
{
	return;
	int CurrentWindow;// = glutGetWindow();

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

void EditorWindowManager::WindowRemoved(int _WindowID)
{
	EditorWindowsToRemove.push_back(_WindowID);
}


void EditorWindowManager::UpdateWindows()
{
	for (auto it : EditorWindows)
	{
		if (IsRemovedID(it.first))
			continue;
		EditorWindow* CurrentWindow = it.second;
		if (CurrentWindow)
			CurrentWindow->UpdateWindow();
	}

	for (int WindowID : EditorWindowsToRemove)
	{
		EditorWindows.erase(WindowID);
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
