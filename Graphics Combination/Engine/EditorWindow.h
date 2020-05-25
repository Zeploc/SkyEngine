#pragma once

#include <glm/glm.hpp>
#include <string>

class EditorWindow
{
public:
	EditorWindow(std::string _WindowName, int _ParentWindow = -1, glm::vec2 _Size = glm::vec2(100, 100), glm::vec2 _Position = glm::vec2(0, 0));
	
	~EditorWindow();

	int GetWindowID() { return WindowID; };
	std::string GetWindowName() { return WindowName; };


	void MainWindowSizeChanged(int _w, int _h);
	void UpdateWindow();
	void RenderWindow();

protected:


public:
	glm::vec2 Size = glm::vec2(100, 100);
	glm::vec2 Position = glm::vec2(0, 0);

	glm::vec3 ClearColour = glm::vec3(1, 1, 1);

protected:

	std::string WindowName;
	int WindowID = -1;

};

void EditorTestProcessNormalKeysDown(unsigned char key, int x, int y);
