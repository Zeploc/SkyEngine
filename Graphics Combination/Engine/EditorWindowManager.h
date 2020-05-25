
#include <map>
#include <glm/glm.hpp>

#pragma once
class EditorWindowManager
{
public:
	EditorWindowManager();
	~EditorWindowManager();

	static class EditorWindow * GetCurrentWindow();

	static std::map<int, class EditorWindow*> GetWindows() { return EditorWindows; };
protected:

public:
	static void StaticRenderWindow();
	static void NewWindowCreated(EditorWindow* _window);
	
	static void UpdateWindows();
	static void MainWindowSizeChanged(int _w, int _h);

protected:

	static std::map<int, EditorWindow*> EditorWindows;

	static EditorWindow* CurrentDraggingWindow;
	static glm::vec2 DragOffset;
};

