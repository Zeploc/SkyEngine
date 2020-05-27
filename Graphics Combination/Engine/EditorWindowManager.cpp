#include "EditorWindowManager.h"

#include "EditorWindow.h"
#include "Input.h"

#include <glew.h>
#include <GLFW/glfw3.h>
//#include <freeglut.h>
#include <iostream>


std::vector<EditorWindow*> EditorWindowManager::EditorWindows;
std::vector<EditorWindow*> EditorWindowManager::EditorWindowsToRemove;
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

bool EditorWindowManager::IsRemovedID(EditorWindow* _Window)
{
	for (EditorWindow* window : EditorWindowsToRemove)
	{
		if (window == _Window)
			return true;
	}
	return false;
}


void EditorWindowManager::NewWindowCreated(EditorWindow * _window)
{
	EditorWindows.push_back(_window);
}

void EditorWindowManager::WindowRemoved(EditorWindow* _Window)
{
	EditorWindowsToRemove.push_back(_Window);
}


void EditorWindowManager::UpdateWindows()
{
	for (auto it : EditorWindows)
	{
		EditorWindow* CurrentWindow = it;
		if (IsRemovedID(CurrentWindow))
			continue;
		if (CurrentWindow)
			CurrentWindow->UpdateWindow();
	}

	auto it = EditorWindows.begin();
	while (it != EditorWindows.end())
	{
		if (IsRemovedID(*it))
		{
			it = EditorWindows.erase(it);
		}
		else
			it++;
	}

}

void EditorWindowManager::RenderWindows()
{
	for (auto it : EditorWindows)
	{
		EditorWindow* CurrentWindow = it;
		if (CurrentWindow)
			CurrentWindow->RenderWindow();
	}
}

void EditorWindowManager::MainWindowSizeChanged(int _w, int _h)
{
	for (auto it : EditorWindows)
	{
		EditorWindow* CurrentWindow = it;
		if (CurrentWindow)
			CurrentWindow->MainWindowSizeChanged(_w, _h);
	}
}
