// Copyright Skyward Studios, Inc. All Rights Reserved.

#include "EditorWindowManager.h"

#include "EditorWindow.h"

#include <glew.h>
#include <GLFW/glfw3.h>
//#include <freeglut.h>
#include <iostream>

std::vector<EditorWindow*> EditorWindowManager::EditorWindows;
std::vector<EditorWindow*> EditorWindowManager::EditorWindowsToRemove;
GLFWwindow* EditorWindowManager::MainWindow = nullptr;
GLFWwindow* EditorWindowManager::CurrentFocused = nullptr;

EditorWindowManager::EditorWindowManager()
{
}

EditorWindowManager::~EditorWindowManager()
{
}

bool EditorWindowManager::IsRemovedID(EditorWindow* _Window)
{
	for (EditorWindow* window : EditorWindowsToRemove)
	{
		if (window == _Window)
		{
			return true;
		}
	}
	return false;
}

void EditorWindowManager::NewWindowCreated(EditorWindow* _window)
{
	EditorWindows.push_back(_window);
	if (_window->GetParentWindow() != MainWindow)
	{
		glfwSetWindowFocusCallback(_window->GetParentWindow(), FocusChanged);
	}
}

void EditorWindowManager::WindowRemoved(EditorWindow* _Window)
{
	EditorWindowsToRemove.push_back(_Window);
}

void EditorWindowManager::UpdateWindows()
{
	/*if (CurrentFocused != MainWindow && CurrentFocused)
	{
		int x, y, width, height;
		glfwGetWindowPos(CurrentFocused, &x, &y);
		glfwGetWindowSize(CurrentFocused, &width, &height);
		int MainX, MainY, MainWidth, MainHeight;
		glfwGetWindowPos(MainWindow, &MainX, &MainY);
		
		glfwGetWindowSize(MainWindow, &MainWidth, &MainHeight);

		if (x + width > MainX && x < MainX + MainWidth && y > MainY && y < MainY + MainHeight)
		{
			std::cout << "Window Pos: " << x << ", " << y << std::endl;
		}
	}*/

	for (auto it : EditorWindows)
	{
		EditorWindow* CurrentWindow = it;
		if (IsRemovedID(CurrentWindow))
		{
			continue;
		}
		if (CurrentWindow)
		{
			CurrentWindow->UpdateWindow();
		}
	}

	auto it = EditorWindows.begin();
	while (it != EditorWindows.end())
	{
		if (IsRemovedID(*it))
		{
			it = EditorWindows.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void EditorWindowManager::RenderWindows()
{
	for (auto it : EditorWindows)
	{
		EditorWindow* CurrentWindow = it;
		if (CurrentWindow)
		{
			CurrentWindow->RenderWindow();
		}
	}
}

void EditorWindowManager::MainWindowSizeChanged(int _w, int _h)
{
	for (auto it : EditorWindows)
	{
		EditorWindow* CurrentWindow = it;
		if (CurrentWindow)
		{
			CurrentWindow->MainWindowSizeChanged(_w, _h);
		}
	}
}

void EditorWindowManager::FocusChanged(GLFWwindow* window, int focused)
{
	if (focused == GLFW_TRUE)
	{
		CurrentFocused = window;
	}
}

void EditorWindowManager::SetMainWindow(GLFWwindow* _MainWindow)
{
	MainWindow = _MainWindow;
	CurrentFocused = MainWindow;
}
