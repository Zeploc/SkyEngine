
#include <map>
#include <vector>
#include <glm/glm.hpp>

#pragma once
class EditorWindowManager
{
public:
	EditorWindowManager();
	~EditorWindowManager();

	static void StaticRenderWindow();
	static void NewWindowCreated(class EditorWindow* _window);
	static void WindowRemoved(int _WindowID);

	static void UpdateWindows();
	static void MainWindowSizeChanged(int _w, int _h);

	static class EditorWindow * GetCurrentWindow();

	static std::map<int, class EditorWindow*> GetWindows() { return EditorWindows; };
protected:
	static bool IsRemovedID(int _WindowID);

public:
	static int iMainWindowID;

protected:

	static std::map<int, EditorWindow*> EditorWindows;
	static std::vector<int> EditorWindowsToRemove;

};

