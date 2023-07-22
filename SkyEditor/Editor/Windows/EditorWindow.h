// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <Math/Vector.h>

class UIElement;
class UIImage;

class EditorWindow
{
public:
	EditorWindow(std::string _WindowName, struct GLFWwindow* _ParentWindow = nullptr, glm::vec2 _Size = glm::vec2(100, 100), glm::vec2 _Position = glm::vec2(0, 0));

	void SetupGlutBindings();

	void SetupUI();

	~EditorWindow();

	std::string GetWindowName() { return WindowName; };

	Vector2 GetPosition();

	void SetWindowPosition(Vector2 _position);

	GLFWwindow* GetParentWindow() { return ParentWindow; }

	void MainWindowSizeChanged(int _w, int _h);

	void UpdateWindow();

	void RenderWindow();

	void PopOut();

	void PopIn();

	void StartDrag();

	void StopDrag();

	bool IsPointInWindow(Vector2 _point);

	void SetBackColour(glm::vec3 _Colour);

protected:
public:
protected:
	glm::vec3 BackColour = glm::vec3(1, 1, 1);

	std::string WindowName;
	GLFWwindow* ParentWindow;

	std::shared_ptr<UIImage> BackImage;

	Vector2 Size = glm::vec2(100, 100);
	Vector2 Position = glm::vec2(0, 0);

	Vector2 DragOffset;
	bool DraggingWindow = false;

	std::vector<std::shared_ptr<UIElement>> UIElements;

	bool bCanPopIn = true;
};
