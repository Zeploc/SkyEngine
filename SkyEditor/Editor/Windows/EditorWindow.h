// Copyright Skyward Studios, Inc. All Rights Reserved.

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>

#include <Math/Vector.h>

#include "Core/EngineWindow.h"

class UIElement;
class UIImage;

class EditorWindow
{
public:
	void CreateExternalWindow();

	EditorWindow(std::string InWindowName, TPointer<EngineWindow> InLinkedWindow = nullptr, SVector2 InSize = SVector2(100, 100), SVector2 InPosition = SVector2(0, 0));

	void SetupGlutBindings();

	void SetupUI();

	~EditorWindow();

	std::string GetWindowName() { return WindowName; };

	SVector2 GetPosition();

	void SetWindowPosition(SVector2 _position);

	TPointer<EngineWindow> GetParentWindow() { return LinkedWindow; }

	void MainWindowSizeChanged(int _w, int _h);

	void UpdateWindow();

	void RenderWindow();

	void PopOut();

	void PopIn();

	void StartDrag();

	void StopDrag();

	bool IsPointInWindow(SVector2 _point);

	void SetBackColour(glm::vec3 _Colour);

protected:
public:
protected:
	glm::vec3 BackColour = glm::vec3(1, 1, 1);

	std::string WindowName;
	TPointer<EngineWindow> LinkedWindow;

	TPointer<UIImage> BackImage;

	SVector2 Size = glm::vec2(100, 100);
	SVector2 Position = glm::vec2(0, 0);

	SVector2 DragOffset;
	bool DraggingWindow = false;

	std::vector<TPointer<UIElement>> UIElements;

	bool bCanPopIn = true;
};
