
#include <map>
#include <vector>
#include <glm/glm.hpp>

#pragma once
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

	static class EditorWindow * GetCurrentWindow();

	static std::vector<class EditorWindow*> GetWindows() { return EditorWindows; };
protected:
	static bool IsRemovedID(EditorWindow* _Window);

public:
	static struct GLFWwindow* MainWindow;

protected:

	static std::vector<EditorWindow*> EditorWindows;
	static std::vector<EditorWindow*> EditorWindowsToRemove;

};

