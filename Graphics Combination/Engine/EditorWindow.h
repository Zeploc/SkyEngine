#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

class EditorWindow
{
public:
	EditorWindow(std::string _WindowName, int _ParentWindow = -1, glm::vec2 _Size = glm::vec2(100, 100), glm::vec2 _Position = glm::vec2(0, 0));

	void SetupGlutBindings();
	void SetupUI();
	
	~EditorWindow();

	int GetWindowID() { return WindowID; };
	std::string GetWindowName() { return WindowName; };
	glm::vec2 GetPosition() { return Position; };

	void SetWindowPosition(glm::vec2 _position);


	void MainWindowSizeChanged(int _w, int _h);
	void UpdateWindow();
	void RenderWindow();

	void PopOut();

	bool IsPointInWindow(glm::vec2 _point);

	void TestButtonPressed();
protected:

public:

	glm::vec3 ClearColour = glm::vec3(1, 1, 1);

	//class Input* WindowInput;


protected:

	std::string WindowName;
	int WindowID = -1;

	glm::vec2 Size = glm::vec2(100, 100);
	glm::vec2 Position = glm::vec2(0, 0);

	glm::vec2 DragOffset;
	glm::vec2 PrevMouse;

	std::vector<std::shared_ptr<class UIElement>> UIElements;

};
