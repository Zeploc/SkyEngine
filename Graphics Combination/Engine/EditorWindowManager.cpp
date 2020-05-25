#include "EditorWindowManager.h"

#include "EditorWindow.h"
#include "Input.h"

#include <freeglut.h>
#include <iostream>


std::map<int, EditorWindow*> EditorWindowManager::EditorWindows;
EditorWindow* EditorWindowManager::CurrentDraggingWindow;
glm::vec2 EditorWindowManager::DragOffset;

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

	// Check for sub windows
	int CurrentWindowID = glutGetWindow();
	auto Current = EditorWindows.find(CurrentWindowID);
	//auto theEND = ;
	if (Current != EditorWindows.end())
	{
		EditorWindow* CurrentWindow = Current->second;
		if (CurrentWindow)
		{
			std::string CurrentName = CurrentWindow->GetWindowName();
			//std::cout << CurrentName.c_str() << " Window Active" << std::endl;
			return CurrentWindow;
		}

	}

	// Check for external windows
	for (auto it : EditorWindows)
	{
		EditorWindow* CurrentWindow = it.second;
		if (CurrentWindow)
		{
			std::string CurrentName = CurrentWindow->GetWindowName();
			if (CurrentName == ActiveWindowName)
			{
				//std::cout << CurrentName.c_str() << " Window Active" << std::endl;
				return CurrentWindow;
			}
		}
	}
	
	// No windows, so is main
	//std::cout << "Main Window Active " << std::endl;
	return nullptr;
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
	//if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_FIRST_PRESS)
	//{
	//	for (auto it : EditorWindows)
	//	{
	//		EditorWindow* CurrentWindow = it.second;
	//		if (CurrentWindow->IsPointInWindow(Input::GetInstance()->MousePos))
	//		{
	//			CurrentDraggingWindow = CurrentWindow;
	//			DragOffset = CurrentWindow->GetPosition() - Input::GetInstance()->MousePos;
	//			break;
	//		}
	//	}
	//}
	//else if (Input::GetInstance()->MouseState[Input::MOUSE_LEFT] == Input::InputState::INPUT_FIRST_RELEASE)
	//{
	//	CurrentDraggingWindow = nullptr;
	//}
	//else
	//{
	//	if (CurrentDraggingWindow)
	//	{
	//		glm::vec2 NewPosition = Input::GetInstance()->MousePos + DragOffset;

	//		//std::cout << "Mouse Pos x:" << Input::GetInstance()->MousePos.x << " y: " << Input::GetInstance()->MousePos .y << std::endl;
	//		//CurrentDraggingWindow->SetWindowPosition(NewPosition);
	//	}
	//}


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
