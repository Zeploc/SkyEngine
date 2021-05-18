#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <memory>

class EditorWindow
{
public:
	EditorWindow(std::string _WindowName, struct GLFWwindow* _ParentWindow = nullptr, glm::vec2 _Size = glm::vec2(100, 100), glm::vec2 _Position = glm::vec2(0, 0));

	void SetupGlutBindings();
	void SetupUI();
	
	~EditorWindow();

	std::string GetWindowName() { return WindowName; };
	glm::vec2 GetPosition();

	void SetWindowPosition(glm::vec2 _position);

	GLFWwindow* GetParentWindow() { return ParentWindow; }

	void MainWindowSizeChanged(int _w, int _h);
	void UpdateWindow();
	void RenderWindow();

	void PopOut();
	void PopIn();

	void StartDrag();
	void StopDrag();

	bool IsPointInWindow(glm::vec2 _point);

	void SetBackColour(glm::vec3 _Colour);
protected:

public:




protected:
	glm::vec3 BackColour = glm::vec3(1, 1, 1);

	std::string WindowName;
	GLFWwindow* ParentWindow;

	std::shared_ptr<class UIImage> BackImage;

	glm::vec2 Size = glm::vec2(100, 100);
	glm::vec2 Position = glm::vec2(0, 0);

	glm::vec2 DragOffset;
	bool DraggingWindow = false;

	std::vector<std::shared_ptr<class UIElement>> UIElements;

	bool bCanPopIn = true;
};
