
#pragma once

#include <vector>

class EditorWindowManager
{
public:
	EditorWindowManager();
	~EditorWindowManager();

	static void NewWindowCreated(class EditorWindow* _window);
	static void WindowRemoved(EditorWindow* _Window);

	static void UpdateWindows();
	static void RenderWindows();
	static void MainWindowSizeChanged(int _w, int _h);

	static void FocusChanged(struct GLFWwindow* window, int focused);

	static GLFWwindow * GetCurrentFocus() { return CurrentFocused; };

	static std::vector<class EditorWindow*> GetWindows() { return EditorWindows; };

	static void SetMainWindow(GLFWwindow* _MainWindow);
protected:
	static bool IsRemovedID(EditorWindow* _Window);

public:
	static struct GLFWwindow* MainWindow;

protected:
	static struct GLFWwindow* CurrentFocused;

	static std::vector<EditorWindow*> EditorWindows;
	static std::vector<EditorWindow*> EditorWindowsToRemove;

};

